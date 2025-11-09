/*=============================================================================
X-MOTO WebAssembly

This file is part of X-MOTO.

X-MOTO is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
=============================================================================*/

/**
 * WebAssembly Entry Point for X-Moto
 *
 * This adapts the desktop game's initialization for WebAssembly/Emscripten.
 * Key differences from native:
 * - Uses Emscripten's main loop (not a while loop)
 * - File system uses IndexedDB (via IDBFS)
 * - Limited threading support
 * - No native system dialogs
 */

#include <emscripten.h>
#include <emscripten/html5.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

// X-Moto headers
#include "xmoto/Game.h"
#include "helpers/Log.h"
#include "helpers/Environment.h"
#include "common/XMArgs.h"
#include "helpers/VExcept.h"

// Global game instance
static GameApp* g_gameApp = nullptr;
static bool g_running = true;
static bool g_initialized = false;

/**
 * Main game loop - called every frame by Emscripten
 */
void gameLoop() {
    if (!g_running || !g_initialized || g_gameApp == nullptr) {
        printf("Game loop stopping: running=%d initialized=%d gameApp=%p\n",
               g_running, g_initialized, (void*)g_gameApp);
        emscripten_cancel_main_loop();
        return;
    }

    try {
        // Run one iteration of the game loop
        // TODO: Adapt GameApp::run_loop() for single-frame execution
        // For now, just keep the app alive

        // Handle SDL events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                g_running = false;
                printf("Quit event received\n");
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    g_running = false;
                    printf("ESC pressed - quitting\n");
                }
            }
        }

    } catch (Exception &e) {
        printf("Exception in game loop: %s\n", e.getMsg().c_str());
        g_running = false;
    }
}

/**
 * Initialize the game
 */
bool initGame(int argc, char** argv) {
    printf("===========================================\n");
    printf("X-Moto WebAssembly - Initializing\n");
    printf("===========================================\n");

    try {
        // Initialize environment
        Environment::init();
        printf("Environment initialized\n");

        // Initialize logger
        // Logger will use browser console via Emscripten's printf
        printf("Logger initialized (console output)\n");

        // Parse command line arguments
        XMArguments args;
        try {
            args.parse(argc, argv);

            if (args.isOptHelp()) {
                args.help(argc >= 1 ? argv[0] : "xmoto");
                return false;
            }
        } catch (Exception &e) {
            printf("Error parsing arguments: %s\n", e.getMsg().c_str());
            return false;
        }

        // Create game instance
        printf("Creating GameApp instance...\n");
        g_gameApp = GameApp::instance();

        if (g_gameApp == nullptr) {
            printf("ERROR: Failed to create GameApp instance\n");
            return false;
        }

        // Initialize game (load resources, setup database, etc.)
        printf("Initializing game resources...\n");
        g_gameApp->run_load(argc, argv);

        printf("===========================================\n");
        printf("X-Moto WebAssembly - Ready!\n");
        printf("===========================================\n");

        g_initialized = true;
        return true;

    } catch (Exception &e) {
        printf("FATAL ERROR during initialization: %s\n", e.getMsg().c_str());
        return false;
    } catch (...) {
        printf("FATAL ERROR: Unknown exception during initialization\n");
        return false;
    }
}

/**
 * Cleanup and shutdown
 */
void cleanupGame() {
    printf("X-Moto WebAssembly - Shutting down...\n");

    if (g_gameApp != nullptr) {
        try {
            g_gameApp->run_unload();
            GameApp::destroy();
        } catch (Exception &e) {
            printf("Error during cleanup: %s\n", e.getMsg().c_str());
        }
        g_gameApp = nullptr;
    }

    // Sync file system to IndexedDB before exit
    EM_ASM(
        if (typeof FS !== 'undefined' && FS.syncfs) {
            FS.syncfs(false, function(err) {
                if (err) {
                    console.error('Failed to sync file system:', err);
                } else {
                    console.log('File system synced successfully');
                }
            });
        }
    );

    printf("X-Moto WebAssembly - Shutdown complete\n");
}

/**
 * Main entry point
 */
int main(int argc, char* argv[]) {
    printf("========================================\n");
    printf("X-Moto WebAssembly Port\n");
    printf("Version: 0.6.3-web (Alpha)\n");
    printf("Build: %s %s\n", __DATE__, __TIME__);
    printf("========================================\n");

    // Set up file system persistence
    EM_ASM(
        // Create persistent storage directory
        try {
            if (!FS.analyzePath('/xmoto').exists) {
                console.log('Creating /xmoto directory');
                FS.mkdir('/xmoto');
            }

            console.log('Mounting IDBFS at /xmoto');
            FS.mount(IDBFS, {}, '/xmoto');

            // Sync from IndexedDB to memory (load saved data)
            console.log('Loading saved data from IndexedDB...');
            FS.syncfs(true, function(err) {
                if (err) {
                    console.error('Error loading file system:', err);
                } else {
                    console.log('File system loaded successfully');
                }
            });
        } catch (e) {
            console.error('Error setting up file system:', e);
        }
    );

    // Small delay to let file system sync complete
    // TODO: Make this properly async
    printf("Waiting for file system to initialize...\n");

    // Initialize the game
    if (!initGame(argc, argv)) {
        printf("Failed to initialize game\n");
        cleanupGame();
        return 1;
    }

    // Set up main loop with Emscripten
    // 0 = use browser's requestAnimationFrame (typically 60 FPS)
    // 1 = simulate infinite loop
    printf("Starting main loop...\n");
    emscripten_set_main_loop(gameLoop, 0, 1);

    // This code will only run if the main loop exits
    cleanupGame();

    return 0;
}

// Export C functions for JavaScript to call
extern "C" {

    EMSCRIPTEN_KEEPALIVE
    void pauseGame() {
        printf("Game paused by JavaScript\n");
        emscripten_pause_main_loop();
    }

    EMSCRIPTEN_KEEPALIVE
    void resumeGame() {
        printf("Game resumed by JavaScript\n");
        emscripten_resume_main_loop();
    }

    EMSCRIPTEN_KEEPALIVE
    void stopGame() {
        printf("Game stopped by JavaScript\n");
        g_running = false;
    }

    EMSCRIPTEN_KEEPALIVE
    int isGameRunning() {
        return g_running ? 1 : 0;
    }
}

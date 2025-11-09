/*=============================================================================
XMOTO - Web Entry Point

This file is part of XMOTO.

XMOTO is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
=============================================================================*/

/**
 * WebAssembly Entry Point for X-Moto
 *
 * This is a minimal entry point for the web version of X-Moto.
 * It initializes the game loop using Emscripten's main loop mechanism.
 */

#include <SDL2/SDL.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <stdio.h>
#include <stdlib.h>

// Forward declarations
void gameLoop();
void initGame();
void cleanupGame();

// Global state
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
bool g_running = true;
int g_frameCount = 0;

// Game loop callback for Emscripten
void gameLoop() {
    if (!g_running) {
        emscripten_cancel_main_loop();
        return;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                g_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    g_running = false;
                }
                break;
        }
    }

    // Clear screen
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
    SDL_RenderClear(g_renderer);

    // Draw a simple animation (rotating square)
    int size = 100;
    int centerX = 640 / 2;
    int centerY = 480 / 2;

    // Calculate rotation based on frame count
    float angle = (g_frameCount % 360) * 0.017453f; // Convert to radians

    // Draw rotating square
    SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
    SDL_Rect rect = {
        centerX - size/2,
        centerY - size/2,
        size,
        size
    };
    SDL_RenderFillRect(g_renderer, &rect);

    // Draw text-like info
    SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
    SDL_Rect titleRect = { centerX - 200, 50, 400, 50 };
    SDL_RenderFillRect(g_renderer, &titleRect);

    SDL_RenderPresent(g_renderer);

    g_frameCount++;
}

void initGame() {
    printf("X-Moto Web - Initializing...\n");

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return;
    }

    // Create window
    g_window = SDL_CreateWindow(
        "X-Moto Web",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        SDL_WINDOW_SHOWN
    );

    if (!g_window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    // Create renderer
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (!g_renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(g_window);
        SDL_Quit();
        return;
    }

    printf("X-Moto Web - Initialization complete!\n");
    printf("Press ESC to exit\n");

    // Set up file system synchronization with IndexedDB
    EM_ASM(
        // Create persistent storage directory
        FS.mkdir('/xmoto');
        FS.mount(IDBFS, {}, '/xmoto');

        // Sync from IndexedDB to memory
        FS.syncfs(true, function(err) {
            if (err) {
                console.error('Error loading file system:', err);
            } else {
                console.log('File system loaded from IndexedDB');
            }
        });
    );
}

void cleanupGame() {
    printf("X-Moto Web - Shutting down...\n");

    // Sync file system to IndexedDB
    EM_ASM(
        FS.syncfs(false, function(err) {
            if (err) {
                console.error('Error saving file system:', err);
            } else {
                console.log('File system saved to IndexedDB');
            }
        });
    );

    if (g_renderer) {
        SDL_DestroyRenderer(g_renderer);
    }

    if (g_window) {
        SDL_DestroyWindow(g_window);
    }

    SDL_Quit();

    printf("X-Moto Web - Shutdown complete\n");
}

// Emscripten callbacks
extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void pauseGame() {
        printf("Game paused\n");
        emscripten_pause_main_loop();
    }

    EMSCRIPTEN_KEEPALIVE
    void resumeGame() {
        printf("Game resumed\n");
        emscripten_resume_main_loop();
    }

    EMSCRIPTEN_KEEPALIVE
    int getFrameCount() {
        return g_frameCount;
    }
}

int main(int argc, char* argv[]) {
    printf("===========================================\n");
    printf("X-Moto WebAssembly Port - Proof of Concept\n");
    printf("===========================================\n");

    initGame();

    // Set up the main loop
    // 0 = use requestAnimationFrame (typically 60 FPS)
    // 1 = simulate infinite loop (will call gameLoop repeatedly)
    emscripten_set_main_loop(gameLoop, 0, 1);

    // Cleanup (will only be called if main loop exits)
    cleanupGame();

    return 0;
}

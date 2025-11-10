/*=============================================================================
X-MOTO WebAssembly - Platform Compatibility Header

This file is part of X-MOTO.

Provides compatibility definitions and adaptations for WebAssembly/Emscripten.
Include this in web-specific builds to handle platform differences.
=============================================================================*/

#ifndef __WEB_PLATFORM_H__
#define __WEB_PLATFORM_H__

#ifdef EMSCRIPTEN

#include <emscripten.h>
#include <emscripten/html5.h>

// Platform identification
#define WEB_PLATFORM 1

// Threading compatibility
// Emscripten has limited threading support
#ifdef NO_THREADS
  #define XMOTO_USE_THREADING 0
#else
  #define XMOTO_USE_THREADING 0  // Disabled by default for web
#endif

// File system compatibility
// Web uses virtual file system with IndexedDB persistence
namespace WebFS {
    // Helper to convert paths for web
    inline std::string adaptPath(const std::string& path) {
        // Web paths need to be adapted to virtual FS
        // For now, just ensure paths are absolute and use /xmoto prefix
        if (path.empty()) return path;

        // If already starts with /xmoto, return as-is
        if (path.find("/xmoto") == 0) {
            return path;
        }

        // If absolute path, keep it
        if (path[0] == '/') {
            return path;
        }

        // Otherwise, make it relative to /xmoto
        return "/xmoto/" + path;
    }

    // Sync file system to IndexedDB
    inline void syncFS() {
        EM_ASM(
            if (typeof FS !== 'undefined' && FS.syncfs) {
                FS.syncfs(false, function(err) {
                    if (err) console.error('FS sync error:', err);
                });
            }
        );
    }
}

// Disable features not available or not practical on web
#define XMOTO_NO_SYSTEM_DIALOGS 1
#define XMOTO_NO_NATIVE_VIDEO_RECORDING 1
#define XMOTO_USE_WEB_NETWORKING 1

// Logging compatibility
// On web, logs go to browser console via Emscripten's printf
#ifdef LogInfo
  #undef LogInfo
#endif
#ifdef LogWarning
  #undef LogWarning
#endif
#ifdef LogError
  #undef LogError
#endif

#define LogInfo(msg, ...) printf("[INFO] " msg "\n", ##__VA_ARGS__)
#define LogWarning(msg, ...) printf("[WARN] " msg "\n", ##__VA_ARGS__)
#define LogError(msg, ...) printf("[ERROR] " msg "\n", ##__VA_ARGS__)

// Time compatibility
// Emscripten provides standard time functions, but we might need adaptations
#include <sys/time.h>

// Environment variables
// Limited support on web - use localStorage via JS
namespace WebEnv {
    inline const char* getEnv(const char* name) {
        // Could use EM_ASM to access localStorage
        // For now, return nullptr (will use defaults)
        return nullptr;
    }
}

// Memory hints for web
#define WEB_MEMORY_LIMIT (512 * 1024 * 1024)  // 512MB soft limit
#define WEB_ASSET_CACHE_SIZE (64 * 1024 * 1024)  // 64MB asset cache

// Utility macros
#define WEB_ONLY(x) x
#define NATIVE_ONLY(x)

#else
  // Non-web builds
  #define WEB_ONLY(x)
  #define NATIVE_ONLY(x) x
#endif // EMSCRIPTEN

#endif // __WEB_PLATFORM_H__

/*=============================================================================
X-MOTO WebAssembly - Platform Stubs

This file provides stub implementations for functionality that's not available
or not yet implemented on the web platform.
=============================================================================*/

#ifdef EMSCRIPTEN

#include "web_platform.h"
#include <stdio.h>

// Threading stubs
// Web platform has limited threading support via Web Workers
// For now, we stub out threading to get the build working

namespace WebStubs {

    // Placeholder for thread-related functionality
    void initThreading() {
        printf("[WebStubs] Threading disabled on web platform\n");
    }

    void shutdownThreading() {
        printf("[WebStubs] Threading shutdown (no-op)\n");
    }

    // Placeholder for system-specific functionality
    void initSystemFeatures() {
        printf("[WebStubs] System features initialized (web mode)\n");
    }

    // Network stubs (if needed)
    void initNetworking() {
        printf("[WebStubs] Networking initialized (web mode)\n");
    }
}

#endif // EMSCRIPTEN

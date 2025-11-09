cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(xmoto-web C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Emscripten-specific settings
if(EMSCRIPTEN)
    message(STATUS "Building for WebAssembly with Emscripten")

    # Set output directory
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../public)

    # Emscripten compiler flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL_MIXER=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL_TTF=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL_NET=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_LIBPNG=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_LIBJPEG=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_ZLIB=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_BZIP2=1")

    # Enable pthreads for threading support
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

    # File system support (IDBFS for IndexedDB)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s FORCE_FILESYSTEM=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lidbfs.js")

    # Memory settings (adjust as needed)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s INITIAL_MEMORY=134217728") # 128MB
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ALLOW_MEMORY_GROWTH=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s MAXIMUM_MEMORY=2147483648") # 2GB max

    # Optimization flags for production
    # For development, use -O0 -g for debugging
    # For production, use -O3 -s WASM=1
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -s ASSERTIONS=2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s SAFE_HEAP=1")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s DEMANGLE_SUPPORT=1")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1")
        # Optimize for size
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s AGGRESSIVE_VARIABLE_ELIMINATION=1")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
    endif()

    # Export functions that JavaScript can call
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s EXPORTED_FUNCTIONS=['_main','_malloc','_free']")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap','FS','IDBFS']")

    # Enable exception catching
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s DISABLE_EXCEPTION_CATCHING=0")

    # Link pre.js and post.js for custom JavaScript
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --pre-js ${CMAKE_CURRENT_SOURCE_DIR}/pre.js")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --post-js ${CMAKE_CURRENT_SOURCE_DIR}/post.js")

    # Use shell template
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/shell.html")

    # Preload game assets
    # This will bundle the assets into the .data file
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file ${CMAKE_SOURCE_DIR}/bin@/bin")

    # Generate HTML output
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

else()
    message(FATAL_ERROR "This CMake file is for Emscripten builds only. Use -DCMAKE_TOOLCHAIN_FILE=/path/to/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
endif()

# Include parent source directory
set(XMOTO_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Add the main xmoto source
# Note: This will need to be adjusted based on the actual source structure
# For now, we'll create a minimal wrapper

# Include directories
include_directories(
    ${XMOTO_SRC_DIR}
    ${XMOTO_SRC_DIR}/include
    ${CMAKE_SOURCE_DIR}/vendor/lua/src
    ${CMAKE_SOURCE_DIR}/vendor/chipmunk/include
    ${CMAKE_SOURCE_DIR}/vendor/ode/include
)

# For the initial PoC, create a minimal executable
# Later, we'll link the full xmoto source
add_executable(xmoto-web
    ${CMAKE_CURRENT_SOURCE_DIR}/web_main.cpp
)

# Link libraries
target_link_libraries(xmoto-web
    # Add xmoto libraries here when ready
    # For now, just SDL
)

# Install target
install(TARGETS xmoto-web
    RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../public
)

# Copy additional files
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/xmoto-web.wasm
    ${CMAKE_CURRENT_BINARY_DIR}/xmoto-web.js
    ${CMAKE_CURRENT_BINARY_DIR}/xmoto-web.data
    DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../public
    OPTIONAL
)

cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(xmoto-web C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Emscripten-specific settings
if(NOT EMSCRIPTEN)
    message(FATAL_ERROR "This CMake file is for Emscripten builds only. Use -DCMAKE_TOOLCHAIN_FILE=/path/to/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
endif()

message(STATUS "Building X-Moto for WebAssembly with Emscripten")

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../public)

# Paths
set(XMOTO_ROOT_DIR ${CMAKE_SOURCE_DIR}/../..)
set(XMOTO_SRC_DIR ${XMOTO_ROOT_DIR}/src)
set(XMOTO_VENDOR_DIR ${XMOTO_ROOT_DIR}/vendor)
set(XMOTO_BIN_DIR ${XMOTO_ROOT_DIR}/bin)

# Platform-specific defines
add_definitions(-DEMSCRIPTEN)
add_definitions(-DUSE_OPENGL)
add_definitions(-DENABLE_OPENGL)
add_definitions(-DALLOW_DEV)  # Enable development features

# Disable gettext for now (can add later)
# add_definitions(-DUSE_GETTEXT)

# Base Emscripten compiler flags
set(EMSCRIPTEN_FLAGS "")
list(APPEND EMSCRIPTEN_FLAGS "-s USE_SDL=2")
list(APPEND EMSCRIPTEN_FLAGS "-s USE_SDL_MIXER=2")
list(APPEND EMSCRIPTEN_FLAGS "-s USE_SDL_TTF=2")
list(APPEND EMSCRIPTEN_FLAGS "-s USE_SDL_NET=2")
list(APPEND EMSCRIPTEN_FLAGS "-s USE_LIBPNG=1")
list(APPEND EMSCRIPTEN_FLAGS "-s USE_LIBJPEG=1")
list(APPEND EMSCRIPTEN_FLAGS "-s USE_ZLIB=1")
list(APPEND EMSCRIPTEN_FLAGS "-s USE_BZIP2=1")
list(APPEND EMSCRIPTEN_FLAGS "-s USE_SQLITE3=1")

# Memory settings
list(APPEND EMSCRIPTEN_FLAGS "-s ALLOW_MEMORY_GROWTH=1")
list(APPEND EMSCRIPTEN_FLAGS "-s INITIAL_MEMORY=134217728")  # 128MB
list(APPEND EMSCRIPTEN_FLAGS "-s MAXIMUM_MEMORY=2147483648")  # 2GB
list(APPEND EMSCRIPTEN_FLAGS "-s STACK_SIZE=5242880")  # 5MB stack

# File system
list(APPEND EMSCRIPTEN_FLAGS "-s FORCE_FILESYSTEM=1")
list(APPEND EMSCRIPTEN_FLAGS "-lidbfs.js")

# Exports
list(APPEND EMSCRIPTEN_FLAGS "-s EXPORTED_FUNCTIONS=['_main','_malloc','_free']")
list(APPEND EMSCRIPTEN_FLAGS "-s EXPORTED_RUNTIME_METHODS=['ccall','cwrap','FS','IDBFS']")

# Exception handling
list(APPEND EMSCRIPTEN_FLAGS "-s DISABLE_EXCEPTION_CATCHING=0")

# Async support
list(APPEND EMSCRIPTEN_FLAGS "-s FETCH=1")

# JavaScript integration
list(APPEND EMSCRIPTEN_FLAGS "--pre-js ${CMAKE_CURRENT_SOURCE_DIR}/pre.js")
list(APPEND EMSCRIPTEN_FLAGS "--post-js ${CMAKE_CURRENT_SOURCE_DIR}/post.js")
list(APPEND EMSCRIPTEN_FLAGS "--shell-file ${CMAKE_CURRENT_SOURCE_DIR}/shell.html")

# Asset preloading - commented out for faster builds during development
# Uncomment when assets are needed:
# list(APPEND EMSCRIPTEN_FLAGS "--preload-file ${XMOTO_BIN_DIR}@/bin")

# Debug vs Release flags
if(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Debug build - including debug symbols and assertions")
    list(APPEND EMSCRIPTEN_FLAGS "-O0")
    list(APPEND EMSCRIPTEN_FLAGS "-g3")
    list(APPEND EMSCRIPTEN_FLAGS "-s ASSERTIONS=2")
    list(APPEND EMSCRIPTEN_FLAGS "-s SAFE_HEAP=1")
    list(APPEND EMSCRIPTEN_FLAGS "-s DEMANGLE_SUPPORT=1")
    list(APPEND EMSCRIPTEN_FLAGS "-s GL_DEBUG=1")
else()
    message(STATUS "Release build - optimizing for size and performance")
    list(APPEND EMSCRIPTEN_FLAGS "-O3")
    list(APPEND EMSCRIPTEN_FLAGS "-s WASM=1")
    list(APPEND EMSCRIPTEN_FLAGS "-s AGGRESSIVE_VARIABLE_ELIMINATION=1")
    list(APPEND EMSCRIPTEN_FLAGS "-flto")
endif()

# Generate HTML output
set(CMAKE_EXECUTABLE_SUFFIX ".html")

# Convert list to string and apply flags
string(REPLACE ";" " " EMSCRIPTEN_FLAGS_STR "${EMSCRIPTEN_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EMSCRIPTEN_FLAGS_STR}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EMSCRIPTEN_FLAGS_STR}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EMSCRIPTEN_FLAGS_STR}")

# Include directories
include_directories(
    ${XMOTO_SRC_DIR}
    ${XMOTO_VENDOR_DIR}/lua/src
    ${XMOTO_VENDOR_DIR}/chipmunk/include
    ${XMOTO_VENDOR_DIR}/ode/include
    ${XMOTO_VENDOR_DIR}/libccd/src
    ${XMOTO_VENDOR_DIR}/md5sum
    ${XMOTO_VENDOR_DIR}/glad/include
    ${XMOTO_VENDOR_DIR}/bzip2
    ${CMAKE_CURRENT_SOURCE_DIR}  # For web-specific headers
    ${CMAKE_CURRENT_BINARY_DIR}  # For generated config files
)

# Configure build config header
set(APP_VERSION_MAJOR 0)
set(APP_VERSION_MINOR 6)
set(APP_VERSION_PATCH 3)
set(APP_VERSION_STRING "0.6.3-web")
configure_file(
    ${XMOTO_SRC_DIR}/common/XMBuildConfig.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/common/XMBuildConfig.h
)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Build vendored libraries
message(STATUS "Building vendored libraries...")
add_subdirectory(${XMOTO_VENDOR_DIR}/bzip2 ${CMAKE_CURRENT_BINARY_DIR}/bzip2)
add_subdirectory(${XMOTO_VENDOR_DIR}/libccd ${CMAKE_CURRENT_BINARY_DIR}/libccd)
add_subdirectory(${XMOTO_VENDOR_DIR}/chipmunk ${CMAKE_CURRENT_BINARY_DIR}/chipmunk)
add_subdirectory(${XMOTO_VENDOR_DIR}/lua ${CMAKE_CURRENT_BINARY_DIR}/lua)
add_subdirectory(${XMOTO_VENDOR_DIR}/md5sum ${CMAKE_CURRENT_BINARY_DIR}/md5sum)
add_subdirectory(${XMOTO_VENDOR_DIR}/ode ${CMAKE_CURRENT_BINARY_DIR}/ode)
add_subdirectory(${XMOTO_VENDOR_DIR}/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)

# Source files - simplified subset for initial build
message(STATUS "Configuring source files...")

# Minimal common sources (essential utilities)
set(common_src
    ${XMOTO_SRC_DIR}/common/CRCHash.cpp
    ${XMOTO_SRC_DIR}/common/DBuffer.cpp
    ${XMOTO_SRC_DIR}/common/Image.cpp
    ${XMOTO_SRC_DIR}/common/Locales.cpp
    ${XMOTO_SRC_DIR}/common/Theme.cpp
    ${XMOTO_SRC_DIR}/common/VFileIO.cpp
    ${XMOTO_SRC_DIR}/common/VTexture.cpp
    ${XMOTO_SRC_DIR}/common/VXml.cpp
    ${XMOTO_SRC_DIR}/common/WWW.cpp
    ${XMOTO_SRC_DIR}/common/XMArgs.cpp
    ${XMOTO_SRC_DIR}/common/XMBuild.cpp
    ${XMOTO_SRC_DIR}/common/XMSession.cpp
    ${XMOTO_SRC_DIR}/common/XMSession_default.cpp
    ${XMOTO_SRC_DIR}/common/svn_version.cpp
)

# Database layer
set(db_src
    ${XMOTO_SRC_DIR}/db/xmDatabase.cpp
    ${XMOTO_SRC_DIR}/db/xmDatabase_config.cpp
    ${XMOTO_SRC_DIR}/db/xmDatabase_levels.cpp
    ${XMOTO_SRC_DIR}/db/xmDatabase_profiles.cpp
)

# Drawing library
set(drawlib_src
    ${XMOTO_SRC_DIR}/drawlib/DrawLib.cpp
    ${XMOTO_SRC_DIR}/drawlib/DrawLibOpenGL.cpp
)

# Minimal GUI
set(gui_src
    ${XMOTO_SRC_DIR}/gui/basic/GUI.cpp
    ${XMOTO_SRC_DIR}/gui/specific/GUIXMoto.cpp
)

# Helper utilities
set(helpers_src
    ${XMOTO_SRC_DIR}/helpers/CmdArgumentParser.cpp
    ${XMOTO_SRC_DIR}/helpers/Environment.cpp
    ${XMOTO_SRC_DIR}/helpers/FileCompression.cpp
    ${XMOTO_SRC_DIR}/helpers/Log.cpp
    ${XMOTO_SRC_DIR}/helpers/Random.cpp
    ${XMOTO_SRC_DIR}/helpers/RenderSurface.cpp
    ${XMOTO_SRC_DIR}/helpers/SwapEndian.cpp
    ${XMOTO_SRC_DIR}/helpers/System.cpp
    ${XMOTO_SRC_DIR}/helpers/Text.cpp
    ${XMOTO_SRC_DIR}/helpers/Time.cpp
    ${XMOTO_SRC_DIR}/helpers/VMath.cpp
    ${XMOTO_SRC_DIR}/helpers/utf8.cpp
)

# Image loading
set(image_src
    ${XMOTO_SRC_DIR}/image/tim.cpp
    ${XMOTO_SRC_DIR}/image/tim_io_stdio.cpp
    ${XMOTO_SRC_DIR}/image/tim_jpeg.cpp
    ${XMOTO_SRC_DIR}/image/tim_memory_crt.cpp
    ${XMOTO_SRC_DIR}/image/tim_png.cpp
)

# Minimal states (main menu and playing)
set(states_src
    ${XMOTO_SRC_DIR}/states/GameState.cpp
    ${XMOTO_SRC_DIR}/states/StateManager.cpp
    ${XMOTO_SRC_DIR}/states/StateMainMenu.cpp
    ${XMOTO_SRC_DIR}/states/StateMenu.cpp
)

# Core game logic (minimal set)
set(xmoto_src
    ${XMOTO_SRC_DIR}/xmoto/Game.cpp
    ${XMOTO_SRC_DIR}/xmoto/GameEvents.cpp
    ${XMOTO_SRC_DIR}/xmoto/GameInit.cpp
    ${XMOTO_SRC_DIR}/xmoto/LevelsManager.cpp
    ${XMOTO_SRC_DIR}/xmoto/Renderer.cpp
    ${XMOTO_SRC_DIR}/xmoto/Sound.cpp
    ${XMOTO_SRC_DIR}/xmoto/SysMessage.cpp
    ${XMOTO_SRC_DIR}/xmoto/UserConfig.cpp
)

# Input handling
set(xmoto_input_src
    ${XMOTO_SRC_DIR}/xmoto/input/Input.cpp
    ${XMOTO_SRC_DIR}/xmoto/input/Joystick.cpp
    ${XMOTO_SRC_DIR}/xmoto/input/XMKey.cpp
)

# Scene/Level (core gameplay)
set(xmscene_src
    ${XMOTO_SRC_DIR}/xmscene/Level.cpp
    ${XMOTO_SRC_DIR}/xmscene/Scene.cpp
    ${XMOTO_SRC_DIR}/xmscene/Camera.cpp
)

# Web-specific entry point and compatibility layer
set(web_src
    ${CMAKE_CURRENT_SOURCE_DIR}/web_main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/web_stubs.cpp
)

# Create the executable
message(STATUS "Creating xmoto-web executable...")
add_executable(xmoto-web
    ${web_src}
    ${common_src}
    ${db_src}
    ${drawlib_src}
    ${gui_src}
    ${helpers_src}
    ${image_src}
    ${states_src}
    ${xmoto_src}
    ${xmoto_input_src}
    ${xmscene_src}
)

# Link libraries
target_link_libraries(xmoto-web
    bzip2
    chipmunk
    libccd
    lua
    md5sum
    ode
    glad
)

message(STATUS "===================================================")
message(STATUS "X-Moto WebAssembly build configured successfully")
message(STATUS "===================================================")
message(STATUS "Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Version: ${APP_VERSION_STRING}")
message(STATUS "===================================================")

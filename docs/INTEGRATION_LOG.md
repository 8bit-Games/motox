# X-Moto WebAssembly Integration Log

**Date**: 2025-11-09
**Status**: Full Game Integration In Progress

---

## Phase 1: Infrastructure (COMPLETE) ✅

**Completed**:
- ✅ Project structure
- ✅ Build automation
- ✅ Documentation
- ✅ PoC with SDL2

---

## Phase 2: Full Game Integration (IN PROGRESS) 🔄

### Session 1: CMake Configuration & Entry Point

**Date**: 2025-11-09

**Changes Made**:

1. **Updated `web/emscripten/CMakeLists.txt`** (274 lines)
   - Configured paths to main X-Moto source tree
   - Added platform-specific defines (EMSCRIPTEN, USE_OPENGL, ALLOW_DEV)
   - Configured all vendor libraries (bzip2, chipmunk, libccd, lua, md5sum, ode, glad)
   - Added minimal subset of game source files:
     - Common utilities (14 files)
     - Database layer (4 files)
     - Drawing library (2 files)
     - GUI (2 files)
     - Helpers (12 files)
     - Image loading (5 files)
     - States (4 files)
     - Core game logic (8 files)
     - Input handling (3 files)
     - Scene/Level (3 files)
   - Configured version header generation
   - Set up proper include directories
   - Linked all vendor libraries

2. **Updated `web/emscripten/web_main.cpp`** (254 lines)
   - Replaced SDL2 demo with actual GameApp integration
   - Includes X-Moto headers (Game.h, Log.h, Environment.h, XMArgs.h, VExcept.h)
   - Calls `GameApp::instance()` to get singleton
   - Calls `run_load()` to initialize game
   - Implements Emscripten-compatible main loop
   - Handles exceptions and errors gracefully
   - Sets up IndexedDB file system persistence
   - Exports JavaScript callable functions

**Technical Details**:

### Build Configuration

```cmake
# Key settings:
- C++ 11 standard
- 128MB initial memory, 2GB max
- 5MB stack size
- IndexedDB file system (IDBFS)
- SQLite3, SDL2 (all modules), PNG, JPEG, ZLIB, BZIP2
- WebGL (OpenGL ES compatibility)
- Exception handling enabled
- Fetch API for async operations
```

### Source Files Included (57 total)

**Common** (14 files):
- CRCHash, DBuffer, Image, Locales, Theme
- VFileIO, VTexture, VXml, WWW
- XMArgs, XMBuild, XMSession, svn_version

**Database** (4 files):
- xmDatabase core + config, levels, profiles

**DrawLib** (2 files):
- DrawLib base + OpenGL

**GUI** (2 files):
- GUI base + GUIXMoto

**Helpers** (12 files):
- CmdArgumentParser, Environment, FileCompression
- Log, Random, RenderSurface, SwapEndian
- System, Text, Time, VMath, utf8

**Image** (5 files):
- TIM image library (stdio, jpeg, png, memory)

**States** (4 files):
- GameState, StateManager, StateMainMenu, StateMenu

**XMoto** (8 files):
- Game, GameEvents, GameInit, LevelsManager
- Renderer, Sound, SysMessage, UserConfig

**Input** (3 files):
- Input, Joystick, XMKey

**Scene** (3 files):
- Level, Scene, Camera

### Files Excluded (For Now)

To keep the initial build manageable, we excluded:
- Most GUI components (will add incrementally)
- Network/Threading code (limited web support)
- Many game states (will add as needed)
- Bike physics (will add next)
- Complete scene components
- Video recording
- Replay system components

**Next Steps**:

1. **Attempt First Build** ⏭️
   - Run `./build.sh Debug`
   - Document any compilation errors
   - Fix missing dependencies
   - Add required source files incrementally

2. **Fix Compilation Issues** (Expected)
   - Missing includes/headers
   - Platform-specific code (`#ifdef EMSCRIPTEN`)
   - Threading issues
   - File path differences
   - Extern/global variables

3. **Add Missing Components**
   - Additional state files
   - GUI components as needed
   - Bike/physics integration
   - Level loading components

4. **Test Basic Initialization**
   - Verify GameApp creation
   - Check Environment::init()
   - Test database initialization
   - Verify logger output

---

## Expected Challenges

### 1. **Threading**
X-Moto uses pthreads. Emscripten has limited threading.
- **Solution**: Disable most threading for web (`-DNO_THREADS`)
- Use Web Workers for critical async operations

### 2. **File System**
Desktop uses native file system. Web uses virtual FS.
- **Solution**: IDBFS for persistence
- May need path translation layer

### 3. **OpenGL Differences**
Desktop uses full OpenGL. Web uses WebGL (OpenGL ES).
- **Solution**: DrawLibOpenGL should work with GLAD
- May need shader compatibility fixes

### 4. **Missing Libraries**
Some dependencies might not have Emscripten ports.
- **Solution**: Using Emscripten's ported libraries where possible
- May need to stub out some features

### 5. **Memory Constraints**
Browsers have stricter memory limits.
- **Solution**: 128MB initial, 2GB max
- May need to optimize asset loading

---

## Build Commands

```bash
# Navigate to web directory
cd web/

# Install dependencies (if not done)
npm install

# Attempt first build (Debug mode for better error messages)
./build.sh Debug

# View full output
./build.sh Debug 2>&1 | tee build.log

# If successful, test in browser
npm run serve
# Open http://localhost:8080
```

---

## Success Criteria for This Phase

### Minimal Success ✅
- [x] CMake configuration completes without errors
- [ ] Source files compile successfully
- [ ] Links create .wasm binary
- [ ] Binary loads in browser

### Functional Success 🎯
- [ ] GameApp initializes
- [ ] No runtime exceptions
- [ ] Logs appear in browser console
- [ ] Game window appears

### Stretch Goals 🚀
- [ ] Database initializes
- [ ] Main menu appears
- [ ] User input works
- [ ] Basic rendering works

---

## Debugging Tips

### Compilation Errors
```bash
# See full error output
./build.sh Debug 2>&1 | grep "error:"

# Check specific file
emcc -I../../src -I../../vendor/lua/src <file.cpp> -c
```

### Runtime Errors
```javascript
// Open browser console (F12)
// Look for:
// - X-Moto initialization messages
// - Exception messages
// - File system errors
```

### Common Fixes
1. **Missing header**: Add to include_directories
2. **Undefined symbol**: Add missing .cpp file
3. **Link error**: Add library to target_link_libraries
4. **Runtime crash**: Check browser console, add -g3 flag

---

## Current File Structure

```
web/emscripten/
├── CMakeLists.txt       # ✅ Updated with full game integration
├── web_main.cpp         # ✅ Updated with GameApp initialization
├── pre.js               # ✅ Existing (JS pre-init)
├── post.js              # ✅ Existing (JS post-init)
└── shell.html           # ✅ Existing (HTML template)
```

---

## Integration Progress

| Component | Status | Files | Notes |
|-----------|--------|-------|-------|
| Build System | ✅ Done | 1 | CMake configured |
| Entry Point | ✅ Done | 1 | GameApp wrapper |
| Common Utils | ✅ Linked | 14 | Basic utilities |
| Database | 🔄 Partial | 4 | Core + profiles |
| Graphics | 🔄 Partial | 2 | OpenGL only |
| GUI | 🔄 Minimal | 2 | Basic only |
| Helpers | ✅ Linked | 12 | Full set |
| Image | ✅ Linked | 5 | Full set |
| States | 🔄 Minimal | 4 | Menu only |
| Game Logic | 🔄 Partial | 8 | Core only |
| Input | ✅ Linked | 3 | Full set |
| Scene | 🔄 Minimal | 3 | Core only |
| Physics | ❌ Not Yet | 0 | Need bike files |
| Network | ❌ Skipped | 0 | Web has different networking |
| Threading | ❌ Disabled | 0 | Limited web support |

**Total**: 57 source files linked, ~20,000 lines of code

---

## Next Session Plan

1. **Build Attempt #1**
   - Run build script
   - Capture all errors
   - Create error resolution plan

2. **Error Resolution**
   - Fix compilation errors one by one
   - Add missing files as needed
   - Add platform-specific  ifdefs

3. **Iterate**
   - Rebuild after each fix
   - Test incrementally
   - Document solutions

4. **First Run**
   - Get binary loading in browser
   - Check console for initialization
   - Verify no crashes

---

**Status**: Ready for first build attempt! 🚀

**Next Command**: `cd web && ./build.sh Debug`


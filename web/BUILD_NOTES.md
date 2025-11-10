# X-Moto WebAssembly Build Notes

**Live Document** - Updated during build process

---

## Build Attempt #1 - Preparation

**Date**: 2025-11-09
**Status**: Preparing for first build

### Pre-Build Changes

#### 1. Created Compatibility Headers

**web/emscripten/web_platform.h**:
- Platform identification macros
- Threading compatibility (disabled)
- File system path adaptation helpers
- Logging redirection to console
- Web-specific feature flags
- Memory hints for browser environment

**web/emscripten/web_stubs.cpp**:
- Stub implementations for unavailable features
- Threading placeholder functions
- System feature initialization

#### 2. Updated CMakeLists.txt

**Removed**:
- `thread/XMThreadStats.cpp` - Requires threading infrastructure
  - Depends on XMThread base class
  - Uses SDL_mutex (pthread-based)
  - Not critical for initial build

**Added**:
- `web_stubs.cpp` to build
- Better comments explaining exclusions

**Result**: Now building 56 source files (was 57)

### Current Source File Inventory

| Category | Files | Status |
|----------|-------|--------|
| Common | 14 | ✅ Ready |
| Database | 4 | ✅ Ready |
| DrawLib | 2 | ✅ Ready |
| GUI | 2 | ✅ Ready |
| Helpers | 12 | ✅ Ready |
| Image | 5 | ✅ Ready |
| States | 4 | ✅ Ready |
| XMoto | 8 | ✅ Ready |
| Input | 3 | ✅ Ready |
| Scene | 3 | ✅ Ready |
| **Threading** | **0** | **⏸️ Disabled** |
| **Web Stubs** | **1** | **✅ Ready** |
| **TOTAL** | **58** | - |

### Anticipated Issues

#### Issue #1: Curl/HTTP
**File**: `common/WWW.cpp`
**Problem**: Uses libcurl for HTTP requests
**Solution**: Emscripten has curl support, should work
**Fallback**: Stub out and use Fetch API via JS

#### Issue #2: SQLite
**File**: `db/xmDatabase.cpp`
**Problem**: Uses SQLite3 for database
**Solution**: Emscripten provides SQL.js, should work
**Fallback**: Use IndexedDB directly via JS

#### Issue #3: File Paths
**Files**: Various (VFileIO, Environment, etc.)
**Problem**: File path assumptions from desktop
**Solution**: web_platform.h provides adaptPath()
**Status**: May need more work

#### Issue #4: XML Parsing
**File**: `common/VXml.cpp`
**Problem**: Uses libxml2
**Solution**: Emscripten has libxml2 port
**Status**: Should work

#### Issue #5: OpenGL Calls
**Files**: `drawlib/DrawLibOpenGL.cpp`, `xmoto/Renderer.cpp`
**Problem**: Uses desktop OpenGL, web has WebGL (OpenGL ES)
**Solution**: GLAD should provide compatibility
**Status**: May need shader adjustments

#### Issue #6: Missing Dependencies
**Various files**
**Problem**: Undiscovered dependencies
**Solution**: Add incrementally as errors appear
**Status**: Expected

### Build Strategy

```bash
# Phase 1: Configuration
cd web/
npm install
emcmake cmake emscripten -DCMAKE_BUILD_TYPE=Debug

# Phase 2: Compilation
./build.sh Debug 2>&1 | tee build-log-1.txt

# Phase 3: Analysis
# Read build-log-1.txt
# Identify error patterns:
# - Missing includes
# - Undefined references
# - Link errors

# Phase 4: Fix and Retry
# Fix one category of errors
# Rebuild
# Repeat
```

### Error Resolution Plan

**Priority 1: Missing Includes**
- Add to include_directories in CMakeLists.txt

**Priority 2: Missing Source Files**
- Add to appropriate source file list

**Priority 3: Platform Incompatibilities**
- Add `#ifdef EMSCRIPTEN` guards
- Use stubs from web_stubs.cpp
- Adapt in web_platform.h

**Priority 4: Link Errors**
- Add libraries to target_link_libraries
- Check library availability in Emscripten

### Expected Timeline

- **Build Attempt #1**: 30-60 minutes
  - Capture errors
  - Categorize issues
  - Plan fixes

- **Fix Iteration #1**: 1-2 hours
  - Fix missing includes
  - Add missing source files
  - Add basic stubs

- **Build Attempt #2**: 30 minutes
  - Verify fixes
  - Capture new errors

- **Iterations**: 3-5 cycles
  - Progressively fix issues
  - Add required components

- **Success Target**: Working build within 4-6 hours

---

## Build Commands Reference

```bash
# Check Emscripten
which emcc
emcc --version

# Clean build
./build.sh Debug clean

# Build with full output
./build.sh Debug 2>&1 | tee build-log.txt

# Build verbosely
cd build/Debug
emmake make VERBOSE=1

# Test single file compilation
cd build/Debug
emmake make src/common/Image.cpp.o

# Check for specific errors
grep "error:" build-log.txt
grep "undefined reference" build-log.txt
grep "No such file" build-log.txt
```

---

## Success Criteria

### Build Success
- [ ] CMake configuration completes
- [ ] All source files compile
- [ ] Link creates .wasm binary
- [ ] HTML, JS, WASM files generated

### Load Success
- [ ] Browser loads xmoto-web.html
- [ ] No JavaScript errors
- [ ] WASM module initializes
- [ ] Console shows initialization messages

### Runtime Success
- [ ] GameApp::instance() succeeds
- [ ] Environment::init() succeeds
- [ ] Database created in IDBFS
- [ ] No crashes or exceptions

---

## Next Steps After Build Success

1. **Test in Browser**
   ```bash
   npm run serve
   # Open http://localhost:8080
   ```

2. **Check Console Output**
   - Look for initialization messages
   - Check for errors/warnings
   - Verify file system setup

3. **Add More Features**
   - Gradually add more source files
   - Enable more game states
   - Add bike physics
   - Add level loading

4. **Optimize**
   - Reduce binary size
   - Improve load time
   - Enable asset preloading

---

**Status**: Ready for first build attempt

**Next Command**: `./build.sh Debug 2>&1 | tee build-attempt-1.log`


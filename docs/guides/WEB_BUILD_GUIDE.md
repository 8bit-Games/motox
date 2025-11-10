# X-Moto Web Build Guide

This guide explains how to build the WebAssembly version of X-Moto for browsers.

## Prerequisites

### Required Tools

1. **Emscripten SDK** (latest stable version)
   ```bash
   # Clone and install Emscripten
   git clone https://github.com/emscripten-core/emsdk.git
   cd emsdk
   ./emsdk install latest
   ./emsdk activate latest
   source ./emsdk_env.sh  # Add this to your ~/.bashrc or ~/.zshrc
   ```

2. **CMake** (3.12 or higher)
   ```bash
   # Ubuntu/Debian
   sudo apt install cmake

   # macOS
   brew install cmake
   ```

3. **Node.js and npm** (18.0 or higher)
   ```bash
   # Ubuntu/Debian
   sudo apt install nodejs npm

   # macOS
   brew install node
   ```

4. **Python** (for Emscripten, usually included)

## Build Instructions

### Quick Start

```bash
# Navigate to web directory
cd web/

# Install dependencies
npm install

# Build WebAssembly (Release mode)
npm run build:wasm

# Serve the application
npm run serve

# Open browser to http://localhost:8080
```

### Development Build

For debugging with source maps and assertions:

```bash
npm run build:wasm:debug
npm run serve
```

### Production Build

Optimized for size and performance:

```bash
npm run build:wasm:release
```

## Build Process Details

### What Happens During Build

1. **CMake Configuration**
   - Configures build with Emscripten toolchain
   - Sets up compiler flags for WebAssembly
   - Configures SDL2 ports and dependencies

2. **Compilation**
   - Compiles C++ source to LLVM bitcode
   - Links with SDL2, physics engines, and libraries
   - Generates WebAssembly (.wasm file)

3. **Output Files**
   - `xmoto-web.html` - Main HTML page
   - `xmoto-web.js` - JavaScript loader
   - `xmoto-web.wasm` - WebAssembly binary
   - `xmoto-web.data` - Preloaded game assets

## Troubleshooting

### Emscripten Not Found

```bash
# Make sure Emscripten is activated
source /path/to/emsdk/emsdk_env.sh

# Verify installation
emcc --version
```

### Build Errors

**Memory Issues:**
```bash
# Increase memory limit in CMakeLists.txt
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s INITIAL_MEMORY=268435456")
```

**Missing Libraries:**
```bash
# Install build dependencies
sudo apt install build-essential libsdl2-dev
```

### Runtime Errors

**CORS Issues:**
Use the provided http-server:
```bash
npm run serve
```

Don't open the HTML file directly (`file://`) - WebAssembly requires HTTP.

**Memory Errors:**
The game may need more memory. Check browser console for errors.

## File System (IndexedDB)

X-Moto Web uses IndexedDB for persistent storage:

```javascript
// File system is automatically mounted at /xmoto
// Saves and settings are stored in browser's IndexedDB
// Cleared when browser cache is cleared
```

## Performance Optimization

### Binary Size Reduction

1. **Enable Link-Time Optimization**
   ```cmake
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
   ```

2. **Strip Debug Info**
   ```cmake
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ASSERTIONS=0")
   ```

3. **Optimize for Size**
   ```cmake
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")
   ```

### Runtime Performance

- Use Chrome or Firefox for best performance
- Enable hardware acceleration in browser
- Close other tabs to free up memory
- WebAssembly is typically 50-80% of native performance

## Deployment

### Static Hosting

The web build can be deployed to any static hosting:

```bash
# Build production version
npm run build:wasm:release

# Deploy the 'public' directory to:
# - GitHub Pages
# - Netlify
# - Vercel
# - CloudFlare Pages
# - AWS S3 + CloudFront
```

### Required Files

Deploy these files from `web/public/`:
- index.html (or xmoto-web.html)
- xmoto-web.js
- xmoto-web.wasm
- xmoto-web.data
- manifest.json
- service-worker.js
- assets/ (icons, images)

### MIME Types

Ensure your server sends correct MIME types:
```
.wasm → application/wasm
.js   → application/javascript
.data → application/octet-stream
```

## Advanced Configuration

### Custom Emscripten Flags

Edit `web/emscripten/CMakeLists.txt`:

```cmake
# Enable threading
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# Enable SIMD
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msimd128")

# Enable exceptions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
```

### Asset Preloading

Preload specific assets:

```cmake
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file ${CMAKE_SOURCE_DIR}/bin/Levels@/Levels")
```

## Browser Compatibility

| Browser | Version | Support |
|---------|---------|---------|
| Chrome  | 87+     | ✅ Full |
| Firefox | 78+     | ✅ Full |
| Safari  | 14.1+   | ✅ Full |
| Edge    | 87+     | ✅ Full |
| Mobile  | Varies  | ⚠️ Limited |

### Mobile Browser Limitations

- Touch controls required
- Performance may vary
- Memory limits stricter
- Audio may require user gesture

## Next Steps

- [Backend Setup Guide](./BACKEND_SETUP_GUIDE.md)
- [Mobile Build Guide](./MOBILE_BUILD_GUIDE.md)
- [Contributing Guidelines](../../CONTRIBUTING.md)

## Support

- GitHub Issues: https://github.com/xmoto/xmoto/issues
- Discord: https://discord.gg/W33yTUd
- Website: https://xmoto.org

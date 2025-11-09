# X-Moto Web

WebAssembly port of X-Moto for browsers.

## Quick Start

```bash
# 1. Install Emscripten SDK (one-time setup)
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh  # Run this in each terminal session

# 2. Install Node.js dependencies
cd /path/to/xmoto/web
npm install

# 3. Check that Emscripten is available
npm run check-deps

# 4. Build WebAssembly (first build may take a few minutes)
npm run build:wasm:release

# 5. Start local development server
npm run serve

# 6. Open browser to http://localhost:8080
```

## Development Workflow

```bash
# Debug build (faster, includes debugging symbols)
npm run build:wasm:debug

# Production build (optimized, smaller file size)
npm run build:wasm:release

# Quick development cycle
npm run dev  # Builds debug version and starts server

# Clean build artifacts
npm run clean
```

## Build Script

The build script (`build.sh`) handles the entire CMake/Emscripten build process:

```bash
# Usage: ./build.sh [Debug|Release] [clean]

./build.sh Debug          # Debug build
./build.sh Release        # Release build
./build.sh Debug clean    # Clean then debug build
```

## Project Status

**Current Status**: Proof of Concept (PoC)

The minimal WebAssembly PoC demonstrates:
- ✅ Emscripten build configuration
- ✅ SDL2 initialization
- ✅ Canvas rendering
- ✅ Event handling
- ✅ File system (IndexedDB)
- 🔄 Game integration (in progress)

**Next Steps**:
- Integrate full game code
- Port physics engines
- Implement level loading
- Add touch controls

## Documentation

- [Web Build Guide](../docs/guides/WEB_BUILD_GUIDE.md) - Detailed instructions
- [Modernization Plan](../MODERNIZATION_PLAN.md) - Full roadmap
- [Getting Started](../docs/guides/GETTING_STARTED.md) - Developer guide

## Requirements

### Required
- **Emscripten SDK** (latest stable)
- **CMake** 3.12 or higher
- **Node.js** 18.0 or higher
- **npm** 9.0 or higher

### Recommended
- Modern web browser (Chrome/Firefox/Safari/Edge)
- 8GB+ RAM for building
- Fast internet (first build downloads dependencies)

## File Structure

```
web/
├── build.sh              # Build automation script
├── package.json          # NPM configuration
├── emscripten/          # WebAssembly build files
│   ├── CMakeLists.txt   # CMake configuration
│   ├── web_main.cpp     # Entry point (PoC)
│   ├── shell.html       # HTML template
│   ├── pre.js           # Pre-init JavaScript
│   └── post.js          # Post-init JavaScript
├── public/              # Output directory
│   ├── index.html       # Generated HTML (symlink to xmoto-web.html)
│   ├── xmoto-web.html   # Main HTML file
│   ├── xmoto-web.js     # JavaScript loader
│   ├── xmoto-web.wasm   # WebAssembly binary
│   ├── xmoto-web.data   # Preloaded assets
│   ├── manifest.json    # PWA manifest
│   └── service-worker.js # Offline support
└── build/               # CMake build directory
    ├── Debug/          # Debug builds
    └── Release/        # Release builds
```

## Troubleshooting

### Emscripten not found
```bash
# Make sure Emscripten is activated in your current shell
source /path/to/emsdk/emsdk_env.sh
npm run check-deps
```

### Build fails
```bash
# Try a clean build
npm run clean
npm run build:wasm:debug

# Check Emscripten version (should be 3.1.x or newer)
emcc --version
```

### CORS errors when loading
```bash
# Don't open the HTML file directly (file://)
# Always use the development server
npm run serve
```

### Out of memory during build
```bash
# Reduce parallel jobs
cd build/Debug
emmake make -j2  # Use 2 jobs instead of auto-detect
```

## Browser Support

| Browser | Version | Status |
|---------|---------|--------|
| Chrome  | 87+     | ✅ Full support |
| Firefox | 78+     | ✅ Full support |
| Safari  | 14.1+   | ✅ Full support |
| Edge    | 87+     | ✅ Full support |
| Mobile  | Varies  | ⚠️ Limited (work in progress) |

## Performance

Expected performance (compared to native):
- **WebAssembly**: 50-80% of native speed
- **First load**: 3-10 seconds (downloads .wasm and .data)
- **Subsequent loads**: <1 second (cached)
- **Memory usage**: 128-512MB

## Contributing

See [MODERNIZATION_PLAN.md](../MODERNIZATION_PLAN.md) for the roadmap.

Areas needing help:
- Full game integration
- Performance optimization
- Touch controls
- Mobile testing

## License

GPL-2.0-or-later

Copyright (C) 2005-2025 X-Moto Team

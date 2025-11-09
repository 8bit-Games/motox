# X-Moto Web

WebAssembly port of X-Moto for browsers.

## Quick Start

```bash
# Install dependencies
npm install

# Build WebAssembly
npm run build:wasm

# Serve locally
npm run serve

# Visit http://localhost:8080
```

## Documentation

- [Web Build Guide](../docs/guides/WEB_BUILD_GUIDE.md)
- [Modernization Plan](../MODERNIZATION_PLAN.md)

## Requirements

- Emscripten SDK
- CMake 3.12+
- Node.js 18+

## Scripts

- `npm run build:wasm:debug` - Debug build
- `npm run build:wasm:release` - Production build
- `npm run serve` - Start local server
- `npm run clean` - Clean build files

## Browser Support

- Chrome 87+
- Firefox 78+
- Safari 14.1+
- Edge 87+

## License

GPL-2.0-or-later

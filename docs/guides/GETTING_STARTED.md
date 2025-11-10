# Getting Started with X-Moto Modernization

Welcome to the X-Moto modernization project! This guide will help you get started with the web, mobile, and backend platforms.

## Project Overview

X-Moto is being modernized to support:

- 🌐 **Web Platform** - Play in your browser via WebAssembly
- 📱 **Mobile Apps** - iOS and Android native apps
- ☁️ **Backend API** - Cloud services for levels, scores, and multiplayer

## Quick Links

- [📋 Modernization Plan](../../MODERNIZATION_PLAN.md) - Complete strategy and roadmap
- [🌐 Web Build Guide](./WEB_BUILD_GUIDE.md) - Build WebAssembly version
- [☁️ Backend Setup Guide](./BACKEND_SETUP_GUIDE.md) - Set up API server
- [📱 Mobile Build Guide](./MOBILE_BUILD_GUIDE.md) - Build mobile apps

## Platform Status

| Platform | Status | Progress |
|----------|--------|----------|
| Web (WebAssembly) | 🟡 In Progress | Infrastructure ready |
| Backend API | 🟡 In Progress | Database schema ready |
| Mobile (iOS/Android) | 🟡 Planned | Configuration ready |
| Desktop (existing) | ✅ Stable | Maintained |

## Architecture Overview

```
┌─────────────────────────────────────────────────────────┐
│                    X-Moto Ecosystem                     │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐             │
│  │   Web    │  │  Mobile  │  │ Desktop  │             │
│  │ Browser  │  │iOS/Android│ │ Native   │             │
│  └────┬─────┘  └────┬─────┘  └────┬─────┘             │
│       │             │             │                     │
│       └─────────────┴─────────────┘                     │
│                     │                                   │
│              ┌──────▼──────┐                           │
│              │ Backend API │                           │
│              │  (Node.js)  │                           │
│              └──────┬──────┘                           │
│                     │                                   │
│       ┌─────────────┼─────────────┐                   │
│       │             │             │                     │
│  ┌────▼────┐  ┌────▼────┐  ┌────▼────┐               │
│  │PostgreSQL│ │  Redis  │  │ Storage │               │
│  │         │  │  Cache  │  │   (S3)  │               │
│  └─────────┘  └─────────┘  └─────────┘               │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

## Directory Structure

```
xmoto/
├── src/                    # Original C++ desktop code
├── web/                    # 🆕 Web platform (WebAssembly)
│   ├── emscripten/        # Build configuration
│   ├── public/            # Static assets
│   └── src/               # Web-specific code
├── backend/               # 🆕 Backend API (Node.js)
│   ├── src/               # API source code
│   ├── database/          # Database schemas
│   └── docker-compose.yml # Docker setup
├── mobile/                # 🆕 Mobile apps (Capacitor)
│   ├── android/           # Android project
│   └── ios/               # iOS project
├── shared/                # 🆕 Shared code/types
└── docs/                  # Documentation
    ├── guides/            # Setup guides
    └── api/               # API documentation
```

## Development Workflow

### 1. Web Development

```bash
# Set up Emscripten
source /path/to/emsdk/emsdk_env.sh

# Build and run
cd web/
npm install
npm run build:wasm:debug
npm run serve

# Visit http://localhost:8080
```

### 2. Backend Development

```bash
cd backend/

# Option A: Docker (Recommended)
npm run docker:up

# Option B: Manual
npm install
npm run dev

# API available at http://localhost:3000
```

### 3. Mobile Development

```bash
cd mobile/
npm install

# Build web assets first
cd ../web
npm run build:wasm:release

# Sync to mobile
cd ../mobile
npm run sync

# Open in IDE
npm run open:ios      # Xcode
npm run open:android  # Android Studio
```

### 4. Desktop Development

```bash
# Existing workflow (unchanged)
mkdir build && cd build
cmake ..
make
./xmoto
```

## Technology Stack

### Frontend
- **WebAssembly** - Compiled C++ code
- **Emscripten** - C++ to WASM compiler
- **SDL2** - Cross-platform API
- **TypeScript** - Web UI code

### Backend
- **Node.js 20+** - Runtime
- **Express** - Web framework
- **PostgreSQL 15+** - Database
- **Redis 7+** - Cache
- **Docker** - Containerization

### Mobile
- **Capacitor** - Native bridge
- **iOS** - Swift (if needed)
- **Android** - Kotlin (if needed)

## First Steps

### For Web Developers

1. Read the [Modernization Plan](../../MODERNIZATION_PLAN.md)
2. Follow the [Web Build Guide](./WEB_BUILD_GUIDE.md)
3. Set up the [Backend API](./BACKEND_SETUP_GUIDE.md)
4. Start contributing!

### For Game Developers

1. Understand the existing C++ codebase
2. Review Emscripten documentation
3. Help port game features to WebAssembly
4. Optimize performance

### For Mobile Developers

1. Set up Capacitor development environment
2. Test web build on mobile devices
3. Implement native mobile features
4. Optimize touch controls

## Development Phases

### Phase 1: Foundation (Current)
- ✅ Project structure
- ✅ Build configurations
- ✅ Database schema
- ✅ Documentation
- 🔄 WebAssembly PoC

### Phase 2: Core Features (Next)
- 🔜 Full web port
- 🔜 Backend API implementation
- 🔜 User authentication
- 🔜 Level management

### Phase 3: Integration
- 🔜 Web-backend integration
- 🔜 Mobile app development
- 🔜 Social features
- 🔜 Cloud sync

### Phase 4: Polish
- 🔜 Performance optimization
- 🔜 UI/UX improvements
- 🔜 Testing
- 🔜 Deployment

## Contributing

We welcome contributions! Here's how to help:

### Code Contributions

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

### Areas Needing Help

- 🎮 WebAssembly game port
- 🎨 Web UI design
- 📱 Mobile app development
- 🔧 Backend API features
- 📝 Documentation
- 🧪 Testing

### Coding Standards

- **C++**: Follow existing style, C++11 standard
- **TypeScript**: ESLint + Prettier
- **SQL**: PostgreSQL conventions
- **Git**: Conventional commits

## Resources

### Documentation
- [Emscripten Docs](https://emscripten.org/docs/)
- [SDL2 Docs](https://wiki.libsdl.org/)
- [Capacitor Docs](https://capacitorjs.com/docs)
- [Express.js Docs](https://expressjs.com/)

### Community
- **Discord**: https://discord.gg/W33yTUd
- **GitHub**: https://github.com/xmoto/xmoto
- **Website**: https://xmoto.org

### Tools
- **Emscripten SDK**: https://emscripten.org/
- **Docker**: https://www.docker.com/
- **VS Code**: Recommended IDE
- **Xcode/Android Studio**: For mobile development

## Troubleshooting

### Common Issues

**Build fails with Emscripten error:**
```bash
# Ensure Emscripten is activated
source /path/to/emsdk/emsdk_env.sh
emcc --version
```

**Backend can't connect to database:**
```bash
# Check Docker services
docker ps
docker logs xmoto-postgres
```

**Mobile app won't sync:**
```bash
# Rebuild web assets first
cd web && npm run build:wasm:release
cd ../mobile && npm run sync
```

### Getting Help

1. Check the documentation
2. Search existing GitHub issues
3. Ask on Discord
4. Create a new issue with details

## Next Steps

Choose your path:

- 🌐 [Build the web version](./WEB_BUILD_GUIDE.md)
- ☁️ [Set up the backend](./BACKEND_SETUP_GUIDE.md)
- 📱 [Build mobile apps](./MOBILE_BUILD_GUIDE.md)
- 📋 [Read the full plan](../../MODERNIZATION_PLAN.md)

---

**Last Updated**: 2025-11-09
**Status**: Active Development
**License**: GPL-2.0-or-later

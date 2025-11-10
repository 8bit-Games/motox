# X-Moto Web/Mobile Modernization Plan

**Version:** 1.0
**Date:** 2025-11-09
**Status:** Initial Planning Phase

## Executive Summary

This document outlines a comprehensive strategy to modernize X-Moto by expanding it to web and mobile platforms while maintaining the existing desktop application. The goal is to reach a broader audience and leverage modern web/mobile technologies.

---

## 1. Current State Assessment

### Existing Architecture
- **Platform:** Desktop-only (Windows, macOS, Linux, FreeBSD)
- **Language:** C++11
- **Graphics:** OpenGL with SDL2
- **Physics:** Chipmunk and ODE engines
- **Build System:** CMake
- **Dependencies:**
  - SDL2 (window, input, audio, networking)
  - SQLite3 (data persistence)
  - CURL (HTTP networking)
  - LibXML2 (level parsing)
  - Image libraries (libjpeg, libpng)

### Key Technical Features
- State machine architecture (~40 game states)
- Physics-based motorcycle platformer
- Level system with strawberry collection mechanics
- Replay recording and playback
- Network features (highscores, level downloads)
- 23-language internationalization
- Level editor and custom level support

### Current Limitations
- No web presence (must download/install)
- No mobile support (iOS/Android)
- Native-only distribution
- Limited reach to casual gamers

---

## 2. Modernization Strategy

### Phase 1: Web Platform (Priority: HIGH)

#### Approach A: WebAssembly Port (RECOMMENDED)
**Technology Stack:**
- Emscripten (C++ to WebAssembly compiler)
- WebGL (OpenGL compatibility)
- SDL2 (Emscripten provides SDL2 port)
- IndexedDB (SQLite3 replacement)
- Web Workers (threading)

**Advantages:**
- ✅ Reuse existing C++ codebase (~95%)
- ✅ Proven technology (Unity, Unreal Engine use this)
- ✅ Performance close to native
- ✅ Physics engines work as-is
- ✅ Faster time to market

**Challenges:**
- ⚠️ File system abstraction needed
- ⚠️ Network layer needs adaptation (fetch API)
- ⚠️ Limited threading (Web Workers)
- ⚠️ Binary size optimization required
- ⚠️ Browser compatibility testing

**Implementation Steps:**
1. Set up Emscripten build system
2. Create CMake configuration for WebAssembly target
3. Port SDL2 initialization to Emscripten
4. Implement file system abstraction (IDBFS)
5. Adapt network layer for browser (fetch API)
6. Create HTML5 canvas wrapper
7. Optimize WASM binary size
8. Build progressive web app (PWA) wrapper
9. Implement web-specific features (touch controls)
10. Deploy to hosting (GitHub Pages, Netlify, or custom)

#### Approach B: Full JavaScript Rewrite
**Technology Stack:**
- TypeScript
- Three.js or PixiJS (WebGL rendering)
- Matter.js or Box2D.js (physics)
- React or Vue (UI framework)

**Advantages:**
- ✅ Modern web development practices
- ✅ Better web ecosystem integration
- ✅ Easier to maintain by web developers
- ✅ Smaller bundle size

**Challenges:**
- ⚠️ Complete rewrite required
- ⚠️ Physics behavior might differ
- ⚠️ Longer development time
- ⚠️ Level format conversion needed

**Recommendation:** **Proceed with Approach A (WebAssembly)** for faster deployment and code reuse.

---

### Phase 2: Mobile Platform (Priority: MEDIUM)

#### Approach A: Cross-Platform Framework (RECOMMENDED)
**Option 1: Flutter**
- Dart language
- Single codebase for iOS/Android
- Good performance with custom rendering
- Growing ecosystem

**Option 2: React Native**
- JavaScript/TypeScript
- Large community
- Native performance
- Requires native modules for OpenGL

**Advantages:**
- ✅ Single codebase for both platforms
- ✅ Faster development
- ✅ Shared UI components
- ✅ Hot reload for development

**Challenges:**
- ⚠️ Need to rewrite game logic
- ⚠️ Physics engine integration
- ⚠️ Performance optimization
- ⚠️ Touch controls design

#### Approach B: Native Development
**iOS:** Swift + Metal/OpenGL
**Android:** Kotlin + OpenGL ES

**Advantages:**
- ✅ Best performance
- ✅ Full platform integration
- ✅ Native look and feel

**Challenges:**
- ⚠️ Duplicate codebases
- ⚠️ Higher maintenance cost
- ⚠️ Longer development time

#### Approach C: WebAssembly Mobile (HYBRID)
- Use Emscripten WebAssembly build
- Wrap in Capacitor or Cordova
- Deploy as hybrid app

**Advantages:**
- ✅ Reuse WebAssembly code
- ✅ Faster to market
- ✅ Single codebase across web/mobile

**Challenges:**
- ⚠️ Performance overhead
- ⚠️ App store approval
- ⚠️ Limited native integration

**Recommendation:** **Start with Approach C (WebAssembly Mobile)** leveraging Phase 1 work, then evaluate native development if needed.

---

### Phase 3: Shared Backend Infrastructure (Priority: HIGH)

#### Backend Services Architecture

**Technology Stack:**
- **API:** Node.js + Express or Python + FastAPI
- **Database:** PostgreSQL or MongoDB
- **Storage:** S3-compatible storage (AWS S3, MinIO)
- **Cache:** Redis
- **Authentication:** OAuth 2.0 / JWT
- **Hosting:** Docker containers on cloud (AWS, GCP, Azure)

**Core Services:**

1. **User Service**
   - Registration/Login
   - Profile management
   - OAuth integration (Google, Discord)
   - JWT token management

2. **Levels Service**
   - Level upload/download
   - Level metadata (difficulty, rating, author)
   - Level search and filtering
   - Level versioning
   - Community ratings/comments

3. **Highscores Service**
   - Score submission with replay verification
   - Leaderboards (global, friends, weekly)
   - Anti-cheat validation
   - Historical records

4. **Replays Service**
   - Replay upload/download
   - Replay validation
   - Replay streaming
   - Replay compression

5. **Social Service**
   - Friends system
   - Challenges between players
   - Achievements/badges
   - Activity feed

6. **CDN Service**
   - Asset delivery (textures, levels)
   - Caching strategy
   - Geographic distribution

**API Design:**
```
Base URL: https://api.xmoto.org/v1

Authentication:
POST   /auth/register
POST   /auth/login
POST   /auth/refresh
GET    /auth/profile

Levels:
GET    /levels                  (list with filters)
GET    /levels/{id}             (specific level)
POST   /levels                  (upload new level)
PUT    /levels/{id}             (update level)
DELETE /levels/{id}             (delete level)
GET    /levels/{id}/download    (download level file)
POST   /levels/{id}/rate        (rate level)
GET    /levels/featured         (featured levels)
GET    /levels/trending         (trending levels)

Highscores:
GET    /levels/{id}/highscores  (leaderboard)
POST   /highscores              (submit score)
GET    /highscores/user/{id}    (user's scores)

Replays:
GET    /replays/{id}            (get replay)
POST   /replays                 (upload replay)
GET    /levels/{id}/replays     (level replays)

Social:
GET    /users/{id}              (user profile)
GET    /users/{id}/friends      (friends list)
POST   /users/{id}/friend       (add friend)
GET    /achievements            (all achievements)
GET    /users/{id}/achievements (user achievements)
```

---

### Phase 4: Web Application Features

#### Progressive Web App (PWA)
- Offline play support
- Install to home screen
- Service worker for caching
- Background sync

#### Web-Specific Features
1. **Touch Controls**
   - Virtual joystick for mobile web
   - Gesture controls
   - Configurable button layout

2. **Social Integration**
   - Share scores on social media
   - Embed levels in websites
   - Discord integration

3. **Level Editor**
   - Browser-based level editor
   - Real-time preview
   - Collaborative editing
   - Template library

4. **Analytics**
   - Player behavior tracking
   - Level completion rates
   - Performance metrics
   - A/B testing framework

---

## 3. Detailed Implementation Roadmap

### Milestone 1: WebAssembly Proof of Concept (4-6 weeks)

**Goal:** Get basic game running in browser

**Tasks:**
- [ ] Install and configure Emscripten SDK
- [ ] Create CMake toolchain for WebAssembly
- [ ] Port main game loop to Emscripten
- [ ] Implement basic canvas rendering
- [ ] Test physics engines in WebAssembly
- [ ] Load and display single level
- [ ] Implement keyboard controls
- [ ] Create minimal HTML wrapper

**Deliverable:** Playable single-level demo in browser

---

### Milestone 2: Full Web Port (8-12 weeks)

**Goal:** Feature-complete web version

**Tasks:**
- [ ] Implement file system using IndexedDB (IDBFS)
- [ ] Port all game states to web
- [ ] Adapt audio system for Web Audio API
- [ ] Implement touch controls
- [ ] Create responsive UI for different screen sizes
- [ ] Port level loading system
- [ ] Implement replay system
- [ ] Add gamepad/controller support
- [ ] Optimize WASM binary size (<5MB)
- [ ] Implement asset streaming/lazy loading
- [ ] Add progress indicators
- [ ] Create settings page

**Deliverable:** Full-featured web version on staging server

---

### Milestone 3: Backend API Development (6-8 weeks)

**Goal:** RESTful API for all game services

**Tasks:**
- [ ] Set up backend project structure
- [ ] Design database schema
- [ ] Implement authentication service
- [ ] Implement levels service
- [ ] Implement highscores service
- [ ] Implement replays service
- [ ] Implement user profiles
- [ ] Add rate limiting and security
- [ ] Create API documentation (Swagger/OpenAPI)
- [ ] Set up monitoring and logging
- [ ] Deploy to staging environment
- [ ] Load testing and optimization

**Deliverable:** Production-ready API

---

### Milestone 4: Web-Backend Integration (4-6 weeks)

**Goal:** Connect web app to backend

**Tasks:**
- [ ] Integrate authentication in web app
- [ ] Implement level browsing/download
- [ ] Implement highscore submission
- [ ] Implement replay upload/download
- [ ] Add social features
- [ ] Implement offline mode with sync
- [ ] Add error handling and retry logic
- [ ] Implement progress synchronization
- [ ] Add analytics tracking
- [ ] User testing and feedback

**Deliverable:** Connected web application

---

### Milestone 5: Mobile App (8-10 weeks)

**Goal:** iOS and Android apps

**Tasks:**
- [ ] Set up Capacitor/Cordova project
- [ ] Integrate WebAssembly build
- [ ] Implement native touch controls
- [ ] Optimize for mobile performance
- [ ] Add mobile-specific UI
- [ ] Implement native notifications
- [ ] Add in-app purchases (optional)
- [ ] Test on various devices
- [ ] Prepare app store assets
- [ ] Submit to app stores

**Deliverable:** Published mobile apps

---

### Milestone 6: Web Level Editor (6-8 weeks)

**Goal:** Browser-based level creation tool

**Tasks:**
- [ ] Design editor UI/UX
- [ ] Implement canvas-based editor
- [ ] Add object placement tools
- [ ] Add physics testing mode
- [ ] Implement save/load functionality
- [ ] Add level validation
- [ ] Create tutorial for new users
- [ ] Add collaborative features
- [ ] Integration testing

**Deliverable:** Web-based level editor

---

## 4. Technical Architecture

### Multi-Platform Structure

```
xmoto/
├── src/                          # Existing C++ desktop code
├── web/
│   ├── emscripten/              # Emscripten build configs
│   │   ├── CMakeLists.txt       # WebAssembly CMake
│   │   ├── pre.js               # Pre-run JavaScript
│   │   ├── post.js              # Post-run JavaScript
│   │   └── shell.html           # HTML shell template
│   ├── public/                  # Static web assets
│   │   ├── index.html
│   │   ├── manifest.json        # PWA manifest
│   │   ├── service-worker.js    # Service worker
│   │   └── assets/              # Images, icons
│   ├── src/                     # Web-specific TypeScript
│   │   ├── ui/                  # UI components
│   │   ├── controls/            # Touch controls
│   │   ├── api/                 # API client
│   │   └── utils/               # Utilities
│   └── package.json             # NPM dependencies
├── mobile/
│   ├── capacitor.config.ts      # Capacitor config
│   ├── android/                 # Android project
│   ├── ios/                     # iOS project
│   └── src/                     # Mobile-specific code
├── backend/
│   ├── src/
│   │   ├── auth/                # Authentication service
│   │   ├── levels/              # Levels service
│   │   ├── highscores/          # Highscores service
│   │   ├── replays/             # Replays service
│   │   ├── users/               # Users service
│   │   └── social/              # Social features
│   ├── database/
│   │   ├── migrations/          # DB migrations
│   │   └── schemas/             # DB schemas
│   ├── tests/                   # API tests
│   ├── docker-compose.yml       # Local dev environment
│   ├── Dockerfile               # Production container
│   └── package.json             # Backend dependencies
├── shared/                       # Shared code/types
│   ├── types/                   # TypeScript types
│   ├── constants/               # Shared constants
│   └── utils/                   # Shared utilities
└── docs/
    ├── api/                     # API documentation
    ├── architecture/            # Architecture docs
    └── guides/                  # Developer guides
```

---

## 5. Technology Stack Summary

### Frontend (Web)
- **Core:** WebAssembly (Emscripten)
- **UI:** TypeScript + React
- **Styling:** CSS Modules or Styled Components
- **State:** Redux or Zustand
- **API Client:** Axios
- **Build:** Webpack or Vite
- **Testing:** Jest + Playwright

### Mobile
- **Framework:** Capacitor (hybrid)
- **Native:** Swift (iOS) + Kotlin (Android) if needed
- **UI:** Same as web with mobile adaptations

### Backend
- **Runtime:** Node.js 20+ or Python 3.11+
- **Framework:** Express.js or FastAPI
- **Database:** PostgreSQL 15+
- **Cache:** Redis 7+
- **Storage:** S3-compatible
- **Auth:** JWT + OAuth 2.0
- **Queue:** BullMQ (for async jobs)
- **Monitoring:** Prometheus + Grafana
- **Logging:** Winston or Pino

### Infrastructure
- **Hosting:** AWS/GCP/Azure or DigitalOcean
- **CDN:** CloudFlare
- **CI/CD:** GitHub Actions
- **Containers:** Docker + Kubernetes (optional)
- **Domains:** xmoto.org + api.xmoto.org

---

## 6. Resource Requirements

### Development Team
- **Full-stack Developer** (1-2): Web/mobile/backend
- **Game Developer** (1): C++/physics/gameplay
- **DevOps Engineer** (0.5-1): Infrastructure/deployment
- **UI/UX Designer** (0.5): Mobile controls/responsive design
- **QA Tester** (0.5): Cross-platform testing

### Infrastructure Costs (Monthly Estimate)
- **Hosting:** $50-200 (VPS/cloud)
- **CDN:** $20-100 (CloudFlare can be free)
- **Database:** $25-100
- **Storage:** $10-50
- **Monitoring:** $20-50
- **Total:** ~$125-500/month

### Timeline
- **Phase 1 (Web PoC):** 1-1.5 months
- **Phase 2 (Full Web):** 2-3 months
- **Phase 3 (Backend):** 2 months
- **Phase 4 (Integration):** 1-1.5 months
- **Phase 5 (Mobile):** 2-2.5 months
- **Phase 6 (Level Editor):** 2 months

**Total:** 10-12 months for full modernization

---

## 7. Risk Assessment

### Technical Risks

| Risk | Probability | Impact | Mitigation |
|------|-------------|--------|------------|
| WebAssembly performance issues | Medium | High | Early PoC, profiling, optimization |
| Physics engine differences | Low | High | Extensive testing, determinism checks |
| Browser compatibility | Medium | Medium | Progressive enhancement, fallbacks |
| App store rejection | Low | Medium | Follow guidelines, prepare appeals |
| Backend scaling | Medium | Medium | Load testing, auto-scaling |
| Security vulnerabilities | Medium | High | Security audits, penetration testing |

### Business Risks

| Risk | Probability | Impact | Mitigation |
|------|-------------|--------|------------|
| Low user adoption | Medium | High | Marketing, community engagement |
| Maintenance overhead | High | Medium | Automation, documentation |
| Community resistance | Low | Low | Clear communication, maintain desktop |
| Funding shortage | Medium | High | Phased approach, sponsorships |

---

## 8. Success Metrics

### Key Performance Indicators (KPIs)

**User Engagement:**
- Monthly Active Users (MAU)
- Daily Active Users (DAU)
- Session length
- Retention rate (Day 1, 7, 30)

**Technical Performance:**
- Page load time (<3s)
- Time to interactive (<5s)
- FPS (60 FPS target)
- Crash rate (<1%)

**Content:**
- Number of levels created
- Level completion rate
- Average level rating
- User-generated content ratio

**Business:**
- Conversion rate (visitor to player)
- Cost per acquisition
- Revenue (if monetized)
- Community growth

---

## 9. Monetization Options (Optional)

1. **Freemium Model**
   - Free core game
   - Premium level packs
   - Cosmetic items (bike skins)

2. **Ads**
   - Non-intrusive banner ads
   - Rewarded video ads for hints

3. **Donations/Sponsorship**
   - Patreon/Open Collective
   - Corporate sponsorships
   - "Buy me a coffee"

4. **Merchandise**
   - T-shirts, stickers
   - Physical game guides

**Recommendation:** Keep free for now, add optional donations

---

## 10. Community & Marketing

### Community Engagement
- Discord server expansion
- Reddit presence (r/gamedev, r/webgames)
- YouTube gameplay/tutorials
- Twitch streaming
- Developer blog

### Launch Strategy
1. **Alpha:** Invite existing players
2. **Beta:** Public beta with feedback
3. **Soft Launch:** Limited regions
4. **Full Launch:** Global release with PR

### Content Strategy
- Weekly level challenges
- User level showcases
- Developer updates
- Behind-the-scenes content
- Speedrun competitions

---

## 11. Next Steps

### Immediate Actions (Week 1)
1. ✅ Review and approve this plan
2. [ ] Set up development environment
3. [ ] Install Emscripten SDK
4. [ ] Create web/ directory structure
5. [ ] Initialize Git branches for web development
6. [ ] Set up project documentation

### Short Term (Month 1)
1. [ ] Complete WebAssembly PoC
2. [ ] Set up backend project structure
3. [ ] Begin API design
4. [ ] Create UI mockups for web
5. [ ] Set up CI/CD for web builds

### Medium Term (Months 2-6)
1. [ ] Complete full web port
2. [ ] Deploy backend API
3. [ ] Beta test with community
4. [ ] Begin mobile development
5. [ ] Create marketing materials

### Long Term (Months 7-12)
1. [ ] Launch web version
2. [ ] Release mobile apps
3. [ ] Launch level editor
4. [ ] Scale infrastructure
5. [ ] Plan future features

---

## 12. Conclusion

This modernization plan will transform X-Moto from a desktop-only game into a cross-platform gaming experience accessible on web and mobile. By leveraging WebAssembly, we can reuse the existing C++ codebase while reaching a much broader audience.

The phased approach allows for incremental development and validation, reducing risk while delivering value at each milestone. The estimated 10-12 month timeline is aggressive but achievable with dedicated resources.

**Recommended First Step:** Begin with Milestone 1 (WebAssembly PoC) to validate the technical approach and demonstrate progress to stakeholders.

---

## Appendix A: Useful Resources

### Emscripten
- [Emscripten Documentation](https://emscripten.org/docs/)
- [SDL2 Port Guide](https://emscripten.org/docs/porting/multimedia_and_graphics/SDL-Games.html)
- [WebAssembly Performance](https://emscripten.org/docs/optimizing/Optimizing-Code.html)

### WebGL
- [WebGL Fundamentals](https://webglfundamentals.org/)
- [Three.js Documentation](https://threejs.org/docs/)

### PWA
- [Progressive Web Apps](https://web.dev/progressive-web-apps/)
- [Service Workers](https://developers.google.com/web/fundamentals/primers/service-workers)

### Mobile
- [Capacitor Documentation](https://capacitorjs.com/docs)
- [React Native](https://reactnative.dev/)
- [Flutter](https://flutter.dev/)

### Backend
- [Express.js Best Practices](https://expressjs.com/en/advanced/best-practice-performance.html)
- [FastAPI Documentation](https://fastapi.tiangolo.com/)
- [PostgreSQL](https://www.postgresql.org/docs/)

---

**Document Version:** 1.0
**Last Updated:** 2025-11-09
**Status:** Awaiting Approval
**Next Review:** After Milestone 1 completion

# X-Moto Backend API

Backend services for X-Moto web and mobile platforms.

## Quick Start

```bash
# Using Docker (recommended)
npm run docker:up

# Manual setup
npm install
npm run dev

# API available at http://localhost:3000
```

## Documentation

- [Backend Setup Guide](../docs/guides/BACKEND_SETUP_GUIDE.md)
- [API Documentation](../docs/api/)

## Features

- User authentication (JWT)
- Level management
- Highscores and leaderboards
- Replay storage
- Social features
- RESTful API

## Tech Stack

- Node.js + Express
- PostgreSQL
- Redis
- TypeScript
- Docker

## Endpoints

- `GET /health` - Health check
- `GET /api/v1` - API info
- `POST /api/v1/auth/*` - Authentication
- `GET /api/v1/levels` - Levels
- `GET /api/v1/highscores` - Highscores
- `GET /api/v1/replays` - Replays

## License

GPL-2.0-or-later

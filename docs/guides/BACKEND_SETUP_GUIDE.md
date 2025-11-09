# X-Moto Backend Setup Guide

This guide explains how to set up and run the X-Moto backend API server.

## Prerequisites

- **Node.js** 18.0 or higher
- **PostgreSQL** 15 or higher
- **Redis** 7 or higher
- **Docker & Docker Compose** (recommended for easy setup)

## Quick Start with Docker

The easiest way to get started is using Docker Compose:

```bash
cd backend/

# Copy environment file
cp .env.example .env

# Edit .env with your settings (optional for local development)
nano .env

# Start all services (PostgreSQL, Redis, API, MinIO)
npm run docker:up

# View logs
npm run docker:logs

# Stop services
npm run docker:down
```

The API will be available at:
- API: http://localhost:3000
- Health check: http://localhost:3000/health
- MinIO Console: http://localhost:9001

## Manual Setup (Without Docker)

### 1. Install Dependencies

```bash
cd backend/
npm install
```

### 2. Set Up PostgreSQL

```bash
# Ubuntu/Debian
sudo apt install postgresql postgresql-contrib

# macOS
brew install postgresql
brew services start postgresql

# Create database and user
psql postgres
CREATE DATABASE xmoto;
CREATE USER xmoto WITH PASSWORD 'changeme';
GRANT ALL PRIVILEGES ON DATABASE xmoto TO xmoto;
\q

# Run initialization script
psql -U xmoto -d xmoto -f database/init.sql
```

### 3. Set Up Redis

```bash
# Ubuntu/Debian
sudo apt install redis-server
sudo systemctl start redis

# macOS
brew install redis
brew services start redis
```

### 4. Configure Environment

```bash
cp .env.example .env
```

Edit `.env` file with your settings:
```env
NODE_ENV=development
PORT=3000

DB_HOST=localhost
DB_PORT=5432
DB_NAME=xmoto
DB_USER=xmoto
DB_PASSWORD=changeme

REDIS_HOST=localhost
REDIS_PORT=6379

JWT_SECRET=your-secret-key-here
```

### 5. Start Development Server

```bash
npm run dev
```

The API will start on http://localhost:3000

## API Endpoints

### Health Check
```bash
curl http://localhost:3000/health
```

### API Root
```bash
curl http://localhost:3000/api/v1
```

### Levels (Sample)
```bash
curl http://localhost:3000/api/v1/levels
```

## Database Migrations

```bash
# Run migrations
npm run migrate

# Rollback migration
npm run migrate:down
```

## Testing

```bash
# Run all tests
npm test

# Watch mode
npm run test:watch

# With coverage
npm test -- --coverage
```

## Production Build

```bash
# Build TypeScript
npm run build

# Start production server
npm start
```

## Docker Deployment

### Build Image

```bash
docker build -t xmoto-backend .
```

### Run Container

```bash
docker run -d \
  --name xmoto-api \
  -p 3000:3000 \
  -e DB_HOST=your-db-host \
  -e DB_PASSWORD=your-db-password \
  -e JWT_SECRET=your-jwt-secret \
  xmoto-backend
```

## Environment Variables

| Variable | Description | Default |
|----------|-------------|---------|
| NODE_ENV | Environment (development/production) | development |
| PORT | API server port | 3000 |
| DB_HOST | PostgreSQL host | localhost |
| DB_PORT | PostgreSQL port | 5432 |
| DB_NAME | Database name | xmoto |
| DB_USER | Database user | xmoto |
| DB_PASSWORD | Database password | changeme |
| REDIS_HOST | Redis host | localhost |
| REDIS_PORT | Redis port | 6379 |
| JWT_SECRET | Secret for JWT tokens | (required) |
| JWT_EXPIRES_IN | Token expiration | 7d |

## Database Schema

The database includes tables for:

- **users** - User accounts and profiles
- **oauth_accounts** - OAuth provider links
- **levels** - Game levels
- **level_ratings** - User ratings and reviews
- **highscores** - Best scores for each level
- **replays** - Replay files
- **friendships** - User friend connections
- **achievements** - Available achievements
- **user_achievements** - Unlocked achievements

## API Documentation

### Authentication

```bash
# Register new user
POST /api/v1/auth/register
{
  "username": "player1",
  "email": "player1@example.com",
  "password": "securepassword"
}

# Login
POST /api/v1/auth/login
{
  "email": "player1@example.com",
  "password": "securepassword"
}
```

### Levels

```bash
# Get all levels
GET /api/v1/levels?difficulty=medium&limit=20

# Get specific level
GET /api/v1/levels/:id

# Upload level (requires auth)
POST /api/v1/levels
Content-Type: multipart/form-data
{
  "name": "My Level",
  "description": "A challenging level",
  "file": <level.lvl>
}
```

### Highscores

```bash
# Get leaderboard for level
GET /api/v1/levels/:levelId/highscores?limit=100

# Submit score (requires auth)
POST /api/v1/highscores
{
  "levelId": "uuid",
  "timeMs": 45000,
  "replayId": "uuid"
}
```

## Security

### Authentication

- JWT tokens with expiration
- bcrypt password hashing
- Rate limiting on auth endpoints
- CORS configuration

### Best Practices

- Never commit `.env` file
- Use strong JWT secrets (32+ characters)
- Enable HTTPS in production
- Use environment-specific configurations
- Regular security updates

## Monitoring

### Health Checks

```bash
# Simple health check
curl http://localhost:3000/health

# Database connectivity check
# (to be implemented)
curl http://localhost:3000/health/db
```

### Logs

Logs are written to:
- Console (stdout/stderr)
- `logs/xmoto.log` (if configured)

## Troubleshooting

### Database Connection Issues

```bash
# Check if PostgreSQL is running
sudo systemctl status postgresql

# Test connection
psql -U xmoto -d xmoto -h localhost
```

### Redis Connection Issues

```bash
# Check if Redis is running
redis-cli ping
# Should return: PONG
```

### Port Already in Use

```bash
# Find process using port 3000
lsof -i :3000

# Kill process
kill -9 <PID>
```

## Development

### Code Structure

```
backend/
├── src/
│   ├── index.ts          # Entry point
│   ├── auth/             # Authentication logic
│   ├── levels/           # Levels endpoints
│   ├── highscores/       # Highscores endpoints
│   ├── replays/          # Replays endpoints
│   ├── users/            # Users endpoints
│   └── social/           # Social features
├── database/
│   ├── migrations/       # Database migrations
│   └── schemas/          # Schema definitions
└── tests/                # API tests
```

### Adding New Endpoints

1. Create route file in appropriate directory
2. Implement controller logic
3. Add validation middleware
4. Register routes in `src/index.ts`
5. Add tests
6. Update API documentation

## Next Steps

- [Web Build Guide](./WEB_BUILD_GUIDE.md)
- [Mobile Build Guide](./MOBILE_BUILD_GUIDE.md)
- [API Reference](../api/)

## Support

- GitHub Issues: https://github.com/xmoto/xmoto/issues
- Discord: https://discord.gg/W33yTUd

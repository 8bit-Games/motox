/**
 * X-Moto Backend API
 * Entry point for the Express application
 */

import express, { Application, Request, Response, NextFunction } from 'express';
import cors from 'cors';
import helmet from 'helmet';
import compression from 'compression';
import dotenv from 'dotenv';
import path from 'path';

// Load environment variables
dotenv.config();

const app: Application = express();
const PORT = process.env.PORT || 3000;
const API_PREFIX = process.env.API_PREFIX || '/api/v1';

// Middleware
app.use(helmet()); // Security headers
app.use(cors({
  origin: process.env.CORS_ORIGIN?.split(',') || '*',
  credentials: process.env.CORS_CREDENTIALS === 'true'
}));
app.use(compression()); // Gzip compression
app.use(express.json({ limit: '10mb' }));
app.use(express.urlencoded({ extended: true, limit: '10mb' }));

// Request logging
app.use((req: Request, res: Response, next: NextFunction) => {
  const start = Date.now();
  res.on('finish', () => {
    const duration = Date.now() - start;
    console.log(`${req.method} ${req.path} ${res.statusCode} ${duration}ms`);
  });
  next();
});

// Health check endpoint
app.get('/health', (req: Request, res: Response) => {
  res.json({
    status: 'ok',
    timestamp: new Date().toISOString(),
    uptime: process.uptime(),
    environment: process.env.NODE_ENV || 'development'
  });
});

// API root
app.get(API_PREFIX, (req: Request, res: Response) => {
  res.json({
    name: 'X-Moto API',
    version: '1.0.0',
    description: 'Backend API for X-Moto game',
    endpoints: {
      health: '/health',
      auth: `${API_PREFIX}/auth`,
      levels: `${API_PREFIX}/levels`,
      highscores: `${API_PREFIX}/highscores`,
      replays: `${API_PREFIX}/replays`,
      users: `${API_PREFIX}/users`
    }
  });
});

// Import routes (to be implemented)
// import authRoutes from './auth/routes';
// import levelsRoutes from './levels/routes';
// import highscoresRoutes from './highscores/routes';
// import replaysRoutes from './replays/routes';
// import usersRoutes from './users/routes';

// Register routes
// app.use(`${API_PREFIX}/auth`, authRoutes);
// app.use(`${API_PREFIX}/levels`, levelsRoutes);
// app.use(`${API_PREFIX}/highscores`, highscoresRoutes);
// app.use(`${API_PREFIX}/replays`, replaysRoutes);
// app.use(`${API_PREFIX}/users`, usersRoutes);

// Placeholder routes (remove when actual routes are implemented)
app.get(`${API_PREFIX}/auth`, (req: Request, res: Response) => {
  res.json({ message: 'Auth endpoints - Coming soon' });
});

app.get(`${API_PREFIX}/levels`, (req: Request, res: Response) => {
  res.json({
    message: 'Levels API',
    sample: [
      { id: 1, name: 'Tutorial', difficulty: 'easy', rating: 4.5 },
      { id: 2, name: 'The Jump', difficulty: 'medium', rating: 4.2 }
    ]
  });
});

app.get(`${API_PREFIX}/highscores`, (req: Request, res: Response) => {
  res.json({ message: 'Highscores endpoints - Coming soon' });
});

app.get(`${API_PREFIX}/replays`, (req: Request, res: Response) => {
  res.json({ message: 'Replays endpoints - Coming soon' });
});

app.get(`${API_PREFIX}/users`, (req: Request, res: Response) => {
  res.json({ message: 'Users endpoints - Coming soon' });
});

// 404 handler
app.use((req: Request, res: Response) => {
  res.status(404).json({
    error: 'Not Found',
    message: `Route ${req.method} ${req.path} not found`
  });
});

// Error handler
app.use((err: Error, req: Request, res: Response, next: NextFunction) => {
  console.error('Error:', err);
  res.status(500).json({
    error: 'Internal Server Error',
    message: process.env.NODE_ENV === 'development' ? err.message : 'Something went wrong'
  });
});

// Start server
app.listen(PORT, () => {
  console.log('='.repeat(50));
  console.log(`X-Moto Backend API`);
  console.log('='.repeat(50));
  console.log(`Environment: ${process.env.NODE_ENV || 'development'}`);
  console.log(`Server running on: http://localhost:${PORT}`);
  console.log(`API base URL: http://localhost:${PORT}${API_PREFIX}`);
  console.log(`Health check: http://localhost:${PORT}/health`);
  console.log('='.repeat(50));
});

export default app;

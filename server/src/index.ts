import express from 'express';
import { Database } from '@vscode/sqlite3';
import * as sqlite from 'sqlite';
import 'dotenv/config';

import { initializeRoutes } from './routes';

const initApp = async () => {
  const app = express();
  const db = await sqlite.open({ filename: 'db.sqlite', driver: Database });
  db.migrate();

  initializeRoutes(app, db);

  const { PORT } = process.env;

  const server = app.listen(PORT, () => {
    console.log(`katla-server listening on port ${PORT} 🔥🐉`);
  });

  // Handle graceful shutdown
  process.on('SIGTERM', async () => {
    console.log('SIGTERM signal received: closing katla-server');
    await db.close();
    server.close(() => {
      console.log('HTTP server closed');
    });
  });
};

initApp();

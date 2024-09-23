import { Database } from 'sqlite';
import express, { Express, Handler } from 'express';
import { keys } from 'ramda';
import path from 'path';

import { createInsertSensorDataHandler } from './resolvers';

type RouteDefinition = {
  path: `/${string}`;
  middleware?: Handler;
} & Partial<Record<'get' | 'post' | 'put', (db: Database) => Handler>>;

const ROUTES: RouteDefinition[] = [
  {
    path: '/',
    get: () => (_, res) => {
      res.sendFile(path.join(__dirname, 'index.html'));
    },
  },
  {
    path: '/api/data',
    post: createInsertSensorDataHandler,
    middleware: express.text(),
  },
];

export const initializeRoutes = (app: Express, db: Database) => {
  app.use(express.static(path.join(__dirname, 'static')));

  ROUTES.forEach(({ path, middleware, ...methods }) => {
    if (middleware) {
      app.use(path, middleware);
    }
    keys(methods).forEach(method => {
      const createResolver = methods[method];
      if (createResolver) {
        app[method](path, createResolver(db));
      }
    });
  });
};

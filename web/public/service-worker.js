/**
 * X-Moto Service Worker
 * Enables offline play and asset caching
 */

const CACHE_NAME = 'xmoto-v1';
const RUNTIME_CACHE = 'xmoto-runtime-v1';

// Assets to cache on install
const PRECACHE_URLS = [
  '/',
  '/index.html',
  '/manifest.json',
  '/xmoto-web.js',
  '/xmoto-web.wasm',
  '/xmoto-web.data',
  '/assets/icon-192.png',
  '/assets/icon-512.png'
];

// Install event - cache core assets
self.addEventListener('install', event => {
  console.log('Service Worker: Installing...');

  event.waitUntil(
    caches.open(CACHE_NAME)
      .then(cache => {
        console.log('Service Worker: Caching core assets');
        return cache.addAll(PRECACHE_URLS);
      })
      .then(() => self.skipWaiting())
  );
});

// Activate event - clean up old caches
self.addEventListener('activate', event => {
  console.log('Service Worker: Activating...');

  event.waitUntil(
    caches.keys()
      .then(cacheNames => {
        return Promise.all(
          cacheNames
            .filter(cacheName => {
              return cacheName !== CACHE_NAME && cacheName !== RUNTIME_CACHE;
            })
            .map(cacheName => {
              console.log('Service Worker: Deleting old cache:', cacheName);
              return caches.delete(cacheName);
            })
        );
      })
      .then(() => self.clients.claim())
  );
});

// Fetch event - serve from cache, fallback to network
self.addEventListener('fetch', event => {
  const { request } = event;
  const url = new URL(request.url);

  // Skip cross-origin requests
  if (url.origin !== location.origin) {
    return;
  }

  // For navigation requests, use network-first strategy
  if (request.mode === 'navigate') {
    event.respondWith(
      fetch(request)
        .catch(() => {
          return caches.match('/index.html');
        })
    );
    return;
  }

  // For game assets (.wasm, .data, .js), use cache-first strategy
  if (request.url.match(/\.(wasm|data|js)$/)) {
    event.respondWith(
      caches.match(request)
        .then(cachedResponse => {
          if (cachedResponse) {
            return cachedResponse;
          }

          return fetch(request)
            .then(response => {
              // Don't cache if not successful
              if (!response || response.status !== 200) {
                return response;
              }

              // Clone the response
              const responseToCache = response.clone();

              caches.open(CACHE_NAME)
                .then(cache => {
                  cache.put(request, responseToCache);
                });

              return response;
            });
        })
    );
    return;
  }

  // For other requests, use cache-first with network fallback
  event.respondWith(
    caches.match(request)
      .then(cachedResponse => {
        if (cachedResponse) {
          // Fetch in background to update cache
          fetch(request)
            .then(response => {
              if (response && response.status === 200) {
                caches.open(RUNTIME_CACHE)
                  .then(cache => {
                    cache.put(request, response);
                  });
              }
            })
            .catch(() => {}); // Ignore errors

          return cachedResponse;
        }

        return fetch(request)
          .then(response => {
            // Don't cache if not successful
            if (!response || response.status !== 200) {
              return response;
            }

            const responseToCache = response.clone();

            caches.open(RUNTIME_CACHE)
              .then(cache => {
                cache.put(request, responseToCache);
              });

            return response;
          });
      })
  );
});

// Message event - handle cache updates
self.addEventListener('message', event => {
  if (event.data && event.data.type === 'SKIP_WAITING') {
    self.skipWaiting();
  }

  if (event.data && event.data.type === 'CLEAR_CACHE') {
    event.waitUntil(
      caches.keys()
        .then(cacheNames => {
          return Promise.all(
            cacheNames.map(cacheName => caches.delete(cacheName))
          );
        })
        .then(() => {
          return self.clients.claim();
        })
    );
  }
});

// Sync event - for background sync when online
self.addEventListener('sync', event => {
  console.log('Service Worker: Background sync:', event.tag);

  if (event.tag === 'sync-highscores') {
    event.waitUntil(syncHighscores());
  }

  if (event.tag === 'sync-replays') {
    event.waitUntil(syncReplays());
  }
});

// Helper functions
async function syncHighscores() {
  // TODO: Implement highscore synchronization
  console.log('Syncing highscores...');
}

async function syncReplays() {
  // TODO: Implement replay synchronization
  console.log('Syncing replays...');
}

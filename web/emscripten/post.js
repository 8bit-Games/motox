/**
 * X-Moto Web - Post-initialization JavaScript
 * This script runs after the WebAssembly module is loaded
 */

console.log('X-Moto Web - Post-initialization');

// Hide loading screen
Module['postRun'] = Module['postRun'] || [];
Module['postRun'].push(function() {
    console.log('X-Moto: Initialization complete');

    // Hide loading screen
    const loadingScreen = document.getElementById('loading-screen');
    if (loadingScreen) {
        loadingScreen.style.display = 'none';
    }

    // Show canvas
    const canvas = document.getElementById('canvas');
    if (canvas) {
        canvas.style.display = 'block';
        canvas.focus();
    }

    // Show controls info
    const controls = document.getElementById('controls-info');
    if (controls) {
        controls.style.display = 'block';
    }

    // Set up periodic file system sync (every 30 seconds)
    setInterval(function() {
        if (typeof FS !== 'undefined' && FS.syncfs) {
            FS.syncfs(false, function(err) {
                if (err) {
                    console.error('Failed to sync file system:', err);
                } else {
                    console.log('File system synced to IndexedDB');
                }
            });
        }
    }, 30000);

    // Add visibility change handler to sync on page hide
    document.addEventListener('visibilitychange', function() {
        if (document.hidden && typeof FS !== 'undefined' && FS.syncfs) {
            FS.syncfs(false, function(err) {
                if (err) {
                    console.error('Failed to sync file system on visibility change:', err);
                }
            });
        }
    });

    // Add beforeunload handler to sync before page closes
    window.addEventListener('beforeunload', function() {
        if (typeof FS !== 'undefined' && FS.syncfs) {
            // Synchronous version for page unload
            FS.syncfs(false, function(err) {});
        }
    });
});

// Expose C++ functions to JavaScript
if (typeof Module !== 'undefined' && Module.ccall) {
    window.XMoto = {
        pause: function() {
            try {
                Module.ccall('pauseGame', null, [], []);
            } catch (e) {
                console.error('Failed to pause game:', e);
            }
        },

        resume: function() {
            try {
                Module.ccall('resumeGame', null, [], []);
            } catch (e) {
                console.error('Failed to resume game:', e);
            }
        },

        getFrameCount: function() {
            try {
                return Module.ccall('getFrameCount', 'number', [], []);
            } catch (e) {
                console.error('Failed to get frame count:', e);
                return 0;
            }
        }
    };

    console.log('XMoto API exposed to window.XMoto');
}

// Set up touch controls for mobile
if ('ontouchstart' in window) {
    console.log('Touch device detected, setting up touch controls');

    // You can add virtual joystick or button overlays here
    const touchControlsDiv = document.getElementById('touch-controls');
    if (touchControlsDiv) {
        touchControlsDiv.style.display = 'block';
        // Add touch control buttons
        // This would be more complex in a real implementation
    }
}

// Set up gamepad support
window.addEventListener('gamepadconnected', function(e) {
    console.log('Gamepad connected:', e.gamepad.id);
});

window.addEventListener('gamepaddisconnected', function(e) {
    console.log('Gamepad disconnected:', e.gamepad.id);
});

// Performance monitoring
if (window.performance && window.performance.memory) {
    setInterval(function() {
        const memory = window.performance.memory;
        console.log('Memory usage:', {
            used: (memory.usedJSHeapSize / 1048576).toFixed(2) + ' MB',
            total: (memory.totalJSHeapSize / 1048576).toFixed(2) + ' MB',
            limit: (memory.jsHeapSizeLimit / 1048576).toFixed(2) + ' MB'
        });
    }, 60000); // Every minute
}

console.log('X-Moto Web ready!');

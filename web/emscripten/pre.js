/**
 * X-Moto Web - Pre-initialization JavaScript
 * This script runs before the WebAssembly module is loaded
 */

console.log('X-Moto Web - Pre-initialization');

// Configure module settings
Module = Module || {};

// Show loading progress
Module['preRun'] = Module['preRun'] || [];
Module['preRun'].push(function() {
    console.log('X-Moto: Starting initialization...');

    // Set up environment variables
    ENV.SDL_EMSCRIPTEN_KEYBOARD_ELEMENT = '#canvas';

    // Configure file system
    if (typeof FS !== 'undefined') {
        console.log('File system available');
    }
});

// Track loading progress
Module['setStatus'] = function(text) {
    if (!Module.setStatus.last) Module.setStatus.last = { time: Date.now(), text: '' };
    if (text === Module.setStatus.last.text) return;

    const m = text.match(/([^(]+)\((\d+(\.\d+)?)\/(\d+)\)/);
    const now = Date.now();

    if (m && now - Module.setStatus.last.time < 30) return; // Only update every 30ms

    Module.setStatus.last.time = now;
    Module.setStatus.last.text = text;

    if (m) {
        const progress = parseInt(m[2]) / parseInt(m[4]) * 100;
        console.log(`Loading: ${progress.toFixed(1)}% - ${m[1]}`);

        // Update UI progress bar if it exists
        const progressBar = document.getElementById('progress-bar');
        if (progressBar) {
            progressBar.style.width = progress + '%';
        }

        const statusText = document.getElementById('status-text');
        if (statusText) {
            statusText.textContent = text;
        }
    } else {
        console.log('Status:', text);
        const statusText = document.getElementById('status-text');
        if (statusText) {
            statusText.textContent = text;
        }
    }
};

// Print output
Module['print'] = function(text) {
    console.log('[stdout]', text);
};

// Print errors
Module['printErr'] = function(text) {
    console.error('[stderr]', text);
};

// Canvas element
Module['canvas'] = (function() {
    const canvas = document.getElementById('canvas');

    // Set up canvas event listeners for mobile
    if (canvas) {
        canvas.addEventListener('webglcontextlost', function(e) {
            alert('WebGL context lost. You will need to reload the page.');
            e.preventDefault();
        }, false);

        // Prevent default touch behaviors to avoid scrolling
        canvas.addEventListener('touchstart', function(e) {
            e.preventDefault();
        }, { passive: false });

        canvas.addEventListener('touchmove', function(e) {
            e.preventDefault();
        }, { passive: false });
    }

    return canvas;
})();

// Focus canvas for keyboard input
Module['onRuntimeInitialized'] = function() {
    const canvas = Module['canvas'];
    if (canvas) {
        canvas.focus();
    }
};

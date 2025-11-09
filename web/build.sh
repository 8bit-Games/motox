#!/bin/bash

# X-Moto WebAssembly Build Script
# This script builds the WebAssembly version of X-Moto

set -e

echo "========================================"
echo "X-Moto WebAssembly Build Script"
echo "========================================"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if Emscripten is available
if ! command -v emcc &> /dev/null; then
    echo -e "${RED}Error: Emscripten not found!${NC}"
    echo "Please install and activate Emscripten SDK:"
    echo "  source /path/to/emsdk/emsdk_env.sh"
    exit 1
fi

echo -e "${GREEN}✓${NC} Emscripten found: $(emcc --version | head -n1)"

# Parse arguments
BUILD_TYPE="${1:-Debug}"
CLEAN="${2:-false}"

if [[ "$BUILD_TYPE" != "Debug" && "$BUILD_TYPE" != "Release" ]]; then
    echo -e "${RED}Error: Build type must be 'Debug' or 'Release'${NC}"
    exit 1
fi

echo "Build type: $BUILD_TYPE"

# Set up directories
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
WEB_DIR="$SCRIPT_DIR"
BUILD_DIR="$WEB_DIR/build/$BUILD_TYPE"
OUTPUT_DIR="$WEB_DIR/public"

# Clean if requested
if [[ "$CLEAN" == "clean" ]]; then
    echo -e "${YELLOW}Cleaning build directory...${NC}"
    rm -rf "$BUILD_DIR"
    rm -f "$OUTPUT_DIR"/xmoto-web.*
    rm -f "$OUTPUT_DIR"/*.data
fi

# Create build directory
mkdir -p "$BUILD_DIR"
mkdir -p "$OUTPUT_DIR"

# Configure with CMake
echo ""
echo "Configuring with CMake..."
cd "$BUILD_DIR"

emcmake cmake ../../emscripten \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DCMAKE_VERBOSE_MAKEFILE=ON

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Configuration failed!${NC}"
    exit 1
fi

echo -e "${GREEN}✓${NC} Configuration successful"

# Build
echo ""
echo "Building..."
NPROC=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
echo "Using $NPROC parallel jobs"

emmake make -j"$NPROC"

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Build failed!${NC}"
    exit 1
fi

echo -e "${GREEN}✓${NC} Build successful"

# Check output files
echo ""
echo "Checking output files..."

REQUIRED_FILES=(
    "$OUTPUT_DIR/xmoto-web.html"
    "$OUTPUT_DIR/xmoto-web.js"
    "$OUTPUT_DIR/xmoto-web.wasm"
)

ALL_FOUND=true
for file in "${REQUIRED_FILES[@]}"; do
    if [ -f "$file" ]; then
        SIZE=$(du -h "$file" | cut -f1)
        echo -e "${GREEN}✓${NC} Found: $(basename "$file") ($SIZE)"
    else
        echo -e "${RED}✗${NC} Missing: $(basename "$file")"
        ALL_FOUND=false
    fi
done

if [ -f "$OUTPUT_DIR/xmoto-web.data" ]; then
    SIZE=$(du -h "$OUTPUT_DIR/xmoto-web.data" | cut -f1)
    echo -e "${GREEN}✓${NC} Found: xmoto-web.data ($SIZE)"
fi

if [ "$ALL_FOUND" = false ]; then
    echo -e "${RED}✗ Build incomplete - missing files!${NC}"
    exit 1
fi

echo ""
echo "========================================"
echo -e "${GREEN}Build completed successfully!${NC}"
echo "========================================"
echo ""
echo "Output files in: $OUTPUT_DIR"
echo ""
echo "To run:"
echo "  cd $WEB_DIR"
echo "  npm run serve"
echo ""
echo "Then open: http://localhost:8080"
echo ""

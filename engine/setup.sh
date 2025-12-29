#!/bin/bash
set -e

# Load emscripten environment if available
if [ -f /etc/profile.d/emscripten.sh ]; then
    source /etc/profile.d/emscripten.sh
fi

# Check if emcc is available
if ! command -v emcc &> /dev/null; then
    echo "Error: emcc not found. Please install Emscripten first."
    echo "Visit: https://emscripten.org/docs/getting_started/downloads.html"
    exit 1
fi

# Create build directory
mkdir -p build

# Output names
OUT=build/stl_viewer.js

# Compile with Emscripten
echo "Compiling with Emscripten..."
emcc -o $OUT main.cpp renderer.cpp model.cpp projection.cpp lighting.cpp rasterizer.cpp \
     -std=c++17 \
     -I./include \
     -s INVOKE_RUN=0 \
     -s 'EXPORTED_RUNTIME_METHODS=["callMain", "FS"]' \
     -s ALLOW_MEMORY_GROWTH=1 \
     -O2

echo "✅ Build complete: $OUT"
echo "The generated files are in build/ directory"
echo "Copy them to ../src/ for the web interface"

#!/bin/bash

# Exit on any error
set -e

echo "=================================="
echo "    SKY RAGE - LINUX BUILDER      "
echo "=================================="

# Check for cmake
if ! command -v cmake &> /dev/null; then
    echo "Error: CMake is not installed."
    echo "Please install it using your package manager (e.g., sudo apt install cmake)."
    exit 1
fi

echo "[1/3] Creating build directory..."
mkdir -p build
cd build

echo "[2/3] Configuring CMake..."
cmake -Wno-dev ..

echo "[3/3] Compiling Project (using all available cores)..."
# Automatically detect number of cores to speed up compilation
CORES=$(nproc 2>/dev/null || echo 4)
make -j$CORES

echo "=================================="
echo " BUILD SUCCESSFUL!                "
echo " You can launch the game via:     "
echo " ./build/bin/SkyRage             "
echo "=================================="

#!/bin/bash

# Exit on error
set -e

echo "=================================="
echo "   SKY RAGE - DOCKER BUILDER      "
echo "=================================="

# Check if docker is installed
if ! command -v docker &> /dev/null; then
    echo "[ERROR] Docker is not installed on your system."
    echo "Please install Docker to use the containerized build process."
    exit 1
fi

echo "[1/2] Building the compiler container image..."
docker build -t skyrage-builder .

echo "[2/2] Compiling the project inside the container..."
# Run the container, mounting the current directory to /workspace
# The CMD in the Dockerfile will execute the cmake/make process automatically
docker run --rm -v "$(pwd):/workspace" skyrage-builder

echo "=================================="
echo " BUILD SUCCESSFUL!                "
echo " The compiled binary is ready at: "
echo " ./build/bin/SkyRage             "
echo "=================================="

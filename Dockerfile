# Use a stable Ubuntu base image
FROM ubuntu:22.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install all required build tools and SFML dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libudev-dev \
    libopenal-dev \
    libvorbis-dev \
    libflac-dev \
    libfreetype6-dev \
    libgl1-mesa-dev \
    libx11-dev \
    libxrandr-dev \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container
WORKDIR /workspace

# Set default command to build the project
CMD ["/bin/bash", "-c", "\
    mkdir -p build && \
    cd build && \
    cmake -Wno-dev .. && \
    make -j$(nproc) \
"]

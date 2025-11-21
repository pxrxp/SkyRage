FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    git cmake build-essential clang \
    libx11-dev libxrandr-dev libxcursor-dev libxi-dev libxinerama-dev \
    libgl1-mesa-dev libopenal-dev libflac-dev libvorbis-dev \
    libfreetype-dev libjpeg-dev libsndfile1-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

COPY . /workspace

# Build SFML, ImGui, ImGui-SFML inside Docker
RUN git clone --branch 2.6.2 https://github.com/SFML/SFML.git /tmp/SFML && \
    mkdir /tmp/SFML/build && cd /tmp/SFML/build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=/usr/local && \
    make -j$(nproc) && make install && rm -rf /tmp/SFML

RUN git clone --branch v1.89 https://github.com/ocornut/imgui.git /opt/imgui

RUN git clone --branch v2.6 https://github.com/SFML/imgui-sfml.git /tmp/imgui-sfml && \
    mkdir /tmp/imgui-sfml/build && cd /tmp/imgui-sfml/build && \
    cmake .. -DIMGUI_DIR=/opt/imgui -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=/usr/local && \
    make -j$(nproc) && make install && rm -rf /tmp/imgui-sfml

RUN mkdir /workspace/build && cd /workspace/build && cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF && make -j$(nproc)

CMD ["/bin/bash"]


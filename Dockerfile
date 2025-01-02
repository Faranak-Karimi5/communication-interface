# Stage 1: Build
FROM ubuntu:20.04 AS build

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary packages and enable 'universe' repository
RUN apt-get update && \
    apt-get install -y software-properties-common && \
    add-apt-repository universe && \
    apt-get update && \
    apt-get install -y \
        build-essential \
        git \
        wget \
        nlohmann-json3-dev \
        libgtest-dev \
        git\
        && rm -rf /var/lib/apt/lists/*

# Install cmake version 3.20 required by cryptopp-cmake repository
RUN apt-get update && \
apt-get install -y software-properties-common wget && \
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null && \
apt-add-repository 'deb https://apt.kitware.com/ubuntu/ focal main' && \
apt-get update && \
apt-get install -y cmake

# Install Google Test

RUN cd /usr/src/gtest && cmake CMakeLists.txt && make && cp lib/*.a /usr/lib

# Set the working directory
WORKDIR /app

# Copy the project files
COPY . /app

# clone the cryptopp-cmake repository 
RUN git clone https://github.com/abdes/cryptopp-cmake.git

# Create a build directory and build the project
RUN mkdir build && cd build && cmake .. && cmake --build . && \
    ls -l /app/build  


# Stage 2: Runtime
FROM ubuntu:20.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary runtime packages
RUN apt-get update && \
    apt-get install -y software-properties-common && \
    add-apt-repository universe && \
    apt-get update && \
    apt-get install -y \
        nlohmann-json3-dev \
        && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /app

# Copy the built application and tests from the build stage
COPY --from=build /app/build/communication_interface /app/communication_interface
COPY --from=build /app/build/runTests /app/runTests


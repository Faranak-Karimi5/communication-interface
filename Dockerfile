# Stage 1: Build
FROM debian:bookworm-slim AS build

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary packages and enable 'universe' repository
RUN apt-get update && \
    apt-get install -y \
        build-essential \
        git \
        wget \
        nlohmann-json3-dev \
        libgtest-dev \
        googletest \
        cmake \
        && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /app

# Copy the project files
COPY . /app

# Create a build directory and build the project
RUN rm -rf build && mkdir build 
RUN cd build && cmake .. 
RUN cd build && cmake --build .

# Stage 2: Runtime
FROM debian:bookworm-slim

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary runtime packages
RUN apt-get update && \
    apt-get install -y \
        build-essential \
        nlohmann-json3-dev \
        libgtest-dev \
        googletest \
        && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /app

# Copy the built application and tests from the build stage
COPY --from=build /app/build/communication_interface /app/communication_interface
COPY --from=build /app/build/runTests /app/runTests


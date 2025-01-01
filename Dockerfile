# Dockerfile

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
        cmake \
        git \
        wget \
        nlohmann-json3-dev \
        libgtest-dev \
        && rm -rf /var/lib/apt/lists/*

# Install Google Test
RUN cd /usr/src/gtest && cmake CMakeLists.txt && make && cp lib/*.a /usr/lib

# Set the working directory
WORKDIR /app

# Copy the project files
COPY . /app

# # Create a build directory and build the project
# RUN mkdir build && cd build && cmake .. && cmake --build .

# Create a build directory and build the project
RUN mkdir build && cd build && cmake .. && cmake --build . && \
    ls -l /app/build  # Add this line to list files


# Stage 2: Runtime
FROM ubuntu:20.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary runtime packages and enable 'universe' repository
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

# Define entrypoints (optional, as Docker Compose will override commands)
# ENTRYPOINT ["/app/communication_interface"]

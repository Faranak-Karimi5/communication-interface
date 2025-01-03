# Communication Interface Class

## Overview

The Communication Interface Class is a robust C++ library designed to facilitate secure communication between multiple devices. It handles data packet manipulation, encoding/decoding using JSON, validation, and supports request-response workflows with AES-CBC encryption. The project leverages modern C++ practices, ensuring modularity, flexibility, and maintainability.

## Features

- Send Control Commands: Securely send validated and encrypted control commands to specific devices, specifying the target device via Device ID.
- Receive Device State: Receive, decrypt, decode, and validate the state information from specific devices by specifying their Device ID.
- Data Encoding/Decoding: Convert data packets to and from JSON format using nlohmann/json, including Device ID for targeted communication.
- Security: Implements AES-CBC encryption and decryption using Crypto++ with a modular security interface.
- Containerized Environment: Utilizes Docker multi-stage builds to ensure a consistent and isolated build and runtime environment.
- Comprehensive Testing: Employs Google Test for thorough unit testing, ensuring all functional requirements are met.
- Static Linking Option: Offers static linking of Crypto++ to simplify deployment and eliminate runtime dependencies.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Assumptions](#assumptions)
- [Requirements Traceability](#requirements-traceability)
- [Design Documentation](#design-documentation)
- [Build and Run Instructions](#build-and-run-instructions)
  - [Using Docker Compose](#using-docker-compose)
    - [Prerequisites](#prerequisites)
    - [Building the Docker Images](#building-the-docker-images)
    - [Running the Communication Interface Service](#running-the-communication-interface-service)
    - [Running the Tests Service](#running-the-tests-service)
    - [Running Both Services Simultaneously](#running-both-services-simultaneously)
  - [Without Docker (Local Build)](#without-docker-local-build)
    - [Prerequisites](#prerequisites-1)
    - [Build Steps](#build-steps)
    - [Run the Application](#run-the-application)
    - [Run the Tests](#run-the-tests)
- [Future Works](#future-works)

## Assumptions
- Data Packets Interpretation: "Data packets" are structured JSON objects focusing on high-level data manipulation.
- Communication Platform: Abstracted away to focus on data processing without implementing real communication protocols.

## Requirements Traceability
Refer to the [Requirements Traceability Matrix](REQUIREMENTS.md) to see how each functional requirement is validated through unit tests.

## Design Documentation
Detailed design and architecture information can be found in the [DESIGN.md](DESIGN.md) file.

## Build and Run Instructions

### Using Docker Compose

#### Prerequisites
- Install [Docker](https://www.docker.com/get-started)
- Install [Docker Compose](https://docs.docker.com/compose/install/)

#### Building the Docker Images
```bash
docker-compose build
```

#### Running the Communication Interface Service
```bash
docker-compose up app
```

#### Running the Tests Service
```bash
docker-compose up tests
```


#### Running Both Services Simultaneously
```bash
docker-compose up
```

### Without Docker (Local Build)

#### Prerequisites
- C++14 compatible compiler
- CMake
- Git

#### Build Steps
```bash
mkdir build
cd build
cmake ..
make
```

#### Run the Application
```bash
./communication_interface
```
#### Run the Tests
```bash
./runTests
```


## Future Works

1. Enhancing Crypto++ Build Process
   
   - Issue:  
     Crypto++ has ceased supporting CMake, which required cloning a community-maintained repository and building it locally. This approach extends the build time.
   
   - Current Fix:  
     The issue has been resolved for subsequent builds on Linux systems.
   
   - Outstanding Challenge:  
     The prolonged build time remains an issue when building using the Dockerfile.


2. Abstracting `CommunicationInterface` for Enhanced Extensibility
   
   - Current Implementation:  
     The CommunicationInterface class currently works with assumption that all devices send JSON data. Since there is no dependency on JSON datatype in interfaces, this class can be extended and inhereted for other data formats
   
   - Proposed Enhancement:  
     Transform CommunicationInterface into an abstract class, allowing for multiple concrete implementations (e.g., CommunicationInterfaceXml, CommunicationInterfaceBinary, etc.).
   
   - Reason for Postponement:  
     This enhancement is deferred as it was not part of the initial core requirements. Focusing on core functionalities ensures timely delivery and stability before introducing additional complexities.

3. Generating and Integrating Doxygen Documentation
   
   - Current State:  
     All method comments are Doxygen-friendly, but the generated documentation is not yet produced, and Doxygen is not installed in the Docker container.

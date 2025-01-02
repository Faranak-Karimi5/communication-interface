# Communication Interface Class

## Overview
A C++ class that facilitates communication between two devices by handling data packet manipulation, encoding/decoding, validation, and supporting request-response workflows with encryption.

## Features
- **Send Control Commands:** Method to send validated and encrypted control commands.
- **Receive Device State:** Method to receive, decrypt, decode, and validate device states.
- **Data Encoding/Decoding:** Converts data to/from JSON format using `nlohmann/json`.
- **Security:** Implements AES-CBC encryption and decryption using a modular security interface.
- **Containerized Environment:** Uses Docker Compose to run the application and its tests as separate services.
- **Comprehensive Testing:** Unit tests mapped to functional requirements using Google Test.

## Assumptions
- **Data Packets Interpretation:** "Data packets" are structured JSON objects focusing on high-level data manipulation.
- **Communication Platform:** Abstracted away to focus on data processing without implementing real communication protocols.

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
- C++11 compatible compiler
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
## Usage Example
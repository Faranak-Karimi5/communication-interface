# Design Documentation

## Architecture Overview

The `CommunicationInterface` class serves as the central component facilitating communication between two devices. It handles data packet manipulation, encoding/decoding, and manages request-response workflows.

## Class Diagram

![CommunicationInterface Class Diagram](docs/class_diagram.png)

## Design Patterns Utilized

- **Singleton Pattern:** Ensures that only one instance of `CommunicationInterface` exists throughout the application lifecycle.
- **Factory Pattern:** Used for creating different types of control commands dynamically.
- **Observer Pattern:** Allows the system to notify listeners about state changes in the other device.

## Component Interactions

1. **Application Module:** Utilizes `CommunicationInterface` to send control commands and receive states.
2. **Encoding Module:** Handles JSON serialization and deserialization using `nlohmann::json`.
3. **Security Module (Optional):** Manages encryption and decryption of data packets using `Crypto++`.

## Data Flow

1. **Sending a Command:**
   - The application creates a control command in JSON format.
   - `CommunicationInterface` encodes the JSON data.
   - (Optional) Encrypts the encoded data.
   - Transmits the data to the other device.

2. **Receiving State:**
   - `CommunicationInterface` receives data from the other device.
   - (Optional) Decrypts the data.
   - Decodes the JSON data to retrieve the device state.
   - Notifies observers about the received state.

# Design Documentation

## Architecture Overview

The Communication Interface Class is designed following a modular architecture that separates concerns across different components. The primary components include:

- CommunicationInterface:  
  Acts as the central hub for sending and receiving data. It handles encoding/decoding, validation, and interacts with the security module for encryption and decryption.

- ISecurity Interface:  
  An abstract interface defining encryption and decryption methods. This allows for flexibility in swapping out different security implementations without altering the CommunicationInterface.

- AESCBCSecurity:  
  A concrete implementation of the ISecurity interface using AES-CBC encryption provided by Crypto++.

- DataPacket Structures (Command and State):  
  Structs representing the data being sent and received. They include validation methods to ensure data integrity.

- Unit Tests:  
  Comprehensive tests using Google Test to verify the functionality of each component and their interactions.


## Component Details

### CommunicationInterface

- Responsibilities:
  - Sending control commands to specified devices.
  - Receiving state information from specified devices.
  - Encoding and decoding data packets.
  - Encrypting and decrypting data using the security module.
  - Handling callbacks for received states.

- Interactions:
  - Utilizes the ISecurity interface for encryption and decryption.
  - Depends on DataPacket structs for structured data representation.

### ISecurity Interface

- Methods:
  - std::string encrypt(const std::string& plaintext): Encrypts plaintext data.
  - std::string decrypt(const std::string& ciphertext): Decrypts ciphertext data.

- Purpose:
  - Provides an abstraction for different encryption mechanisms, promoting flexibility and extensibility.

### AESCBCSecurity

- Implementation:
  - Implements the ISecurity interface using AES-CBC mode from Crypto++.
  
- Features:
  - Handles key management internally.
  - Ensures secure encryption and decryption processes.

### DataPacket Structures

- Command Struct:
  - Fields: commandName, speed, duration.
  - Methods: validate() ensures that command fields meet predefined criteria.

- State Struct:
  - Fields: deviceId, status, value.
  - Methods: validate() ensures that state fields are correctly populated.

## Design Patterns Utilized

- Strategy Pattern:  
  Employed through the ISecurity interface to allow dynamic selection of encryption strategies.



## Thread Safety

- Mutexes:  
  Utilized within CommunicationInterface to protect shared resources and ensure thread-safe operations during send and receive processes.

## Error Handling

- Exception Handling:  
  Implements try-catch blocks around critical operations like validation, encryption, and decryption to handle and log errors gracefully.

- Logging:  
  Logs errors and important events to std::cerr or a designated logging mechanism to aid in debugging and monitoring.

## Extensibility

- Security Modules:  
  Can be easily extended by implementing the ISecurity interface with different encryption algorithms or security protocols.

- Data Formats:  
  Future enhancements can include support for additional data formats like XML or binary by extending the CommunicationInterface.

## Future Enhancements

- Real Networking Integration:  
  Implement actual communication protocols (e.g., TCP/IP, MQTT) for real-time data transmission between devices.

- Advanced Logging Mechanism:  
  Integrate a robust logging framework (e.g., spdlog, Boost.Log) for better log management.

- Device Management Layer:  
  Develop a dedicated module for managing device registrations, statuses, and routing based on Device ID.

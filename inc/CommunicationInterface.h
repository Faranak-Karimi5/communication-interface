// include/CommunicationInterface.h
#ifndef COMMUNICATION_INTERFACE_H
#define COMMUNICATION_INTERFACE_H

#include <string>
#include <functional>
#include <memory>
#include <mutex>
#include "ISecurity.h"
#include "DataPacket.h"

/**
 * @brief CommunicationInterface class handles communication between devices.
 *
 * This class provides methods to send control commands and receive states from other devices.
 * It handles data encoding/decoding, validation, and utilizes a security module for encryption and decryption.
 */
class CommunicationInterface {
public:
    // Constructor and Destructor
    CommunicationInterface(std::unique_ptr<ISecurity> securityModule);
    ~CommunicationInterface();

    // Public Methods
    bool sendControlCommand(const Command& command);
    bool receiveState(State& state);

    // Optional: Set callback for incoming states
    void setStateCallback(std::function<void(const State&)> callback);

private:
    // Data Manipulation Methods
    std::string encodeCommand(const Command& command);
    bool decodeState(const std::string& jsonStr, State& state);

    // Communication Methods (Platform-Agnostic Placeholder)
    bool sendData(const std::string& data);
    bool receiveData(std::string& data);

    // Member Variables
    std::mutex mtx_; // For thread safety
    std::function<void(const State&)> stateCallback_;
    std::unique_ptr<ISecurity> securityModule_; // Security module
};

#endif // COMMUNICATION_INTERFACE_H

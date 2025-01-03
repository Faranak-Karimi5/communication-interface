#ifndef COMMUNICATION_INTERFACE_H
#define COMMUNICATION_INTERFACE_H
// Include Standard Library Headers
#include <string>
#include <functional>
#include <memory>
#include <mutex>

// Include Local Header Files
#include "ISecurity.h"
#include "DataPacket.h" 

// For using the FRIEND_TEST macro
#include <gtest/gtest_prod.h>
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
    bool sendControlCommand(const DataPacket::Command& command);
    bool receiveState(DataPacket::State& state);

    // Set callback for incoming states
    void setStateCallback(std::function<void(const DataPacket::State&)> callback);

private:
    // Data Manipulation Methods
    std::string encodeCommand(const DataPacket::Command& command);
    bool decodeState(const std::string& jsonStr, DataPacket::State& state);

    // Communication Methods (Platform-Agnostic Placeholder)
    bool sendData(const std::string& data);
    bool receiveData(std::string& data);

    // Member Variables
    std::mutex mtx_; // For thread safety
    std::function<void(const DataPacket::State&)> stateCallback_;
    std::unique_ptr<ISecurity> securityModule_; // Security module

    // Grant access to specific test cases
    FRIEND_TEST(CommunicationInterfaceTest, RQ003_EncodeCommand_Success);
    FRIEND_TEST(CommunicationInterfaceTest, RQ004_DecodeState_Success);
};

#endif // COMMUNICATION_INTERFACE_H

// include/CommunicationInterface.h
#ifndef COMMUNICATION_INTERFACE_H
#define COMMUNICATION_INTERFACE_H

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
    /*
     * @brief Sends a control command to a specified device after validating, encoding, and encrypting it.
     *
     * @param deviceId The unique identifier of the target device.
     * @param command The Command object to send.
     * @return true if sending is successful, false otherwise.
     */
    bool sendControlCommand(const std::string& deviceId, const DataPacket::Command& command);

    /*
     * @brief Receives state data from a specified device, decrypts, decodes, and validates it.
     *
     * @param deviceId The unique identifier of the source device.
     * @param state The State object to populate with received data.
     * @return true if receiving and processing is successful, false otherwise.
     */
    bool receiveState(const std::string& deviceId, DataPacket::State& state);

    /*
     * @brief Sets a callback function to handle received states.
     *
     * @param callback A function that takes a const State& as parameter.
     */
    void setStateCallback(std::function<void(const DataPacket::State&)> callback);

private:
    // Data Manipulation Methods
    /*
     * @brief Encodes a Command object to a JSON string with device ID.
     *
     * @param deviceId The unique identifier of the target device.
     * @param command The Command object to encode.
     * @return JSON string representing the Command with device ID.
     */
    std::string encodeCommand(const std::string& deviceId, const DataPacket::Command& command);

    /*
     * @brief Decodes a JSON string to a State object.
     *
     * @param jsonStr The JSON string to decode.
     * @param state The State object to populate.
     * @return true if decoding is successful, false otherwise.
     */
    bool decodeState(const std::string& jsonStr, DataPacket::State& state);

    // Communication Methods (Platform-Agnostic Placeholder)
    /*
     * @brief Placeholder method to simulate sending data to a specific device.
     *
     * @param deviceId The unique identifier of the target device.
     * @param data The encrypted data to send.
     * @return true if sending is successful, false otherwise.
     */
    bool sendData(const std::string& deviceId, const std::string& data);

    /*
     * @brief Placeholder method to simulate receiving data.
     *
     * @param data The encrypted data received.
     * @return true if receiving is successful, false otherwise.
     */
    bool receiveData(std::string& data);

    // Member Variables
    std::mutex mtx_; // For thread safety
    std::function<void(const DataPacket::State&)> stateCallback_;
    std::unique_ptr<ISecurity> securityModule_; // Security module

    // Grant access to specific test cases
    FRIEND_TEST(CommunicationInterfaceTest, RQ003_EncodeCommand_Success);
    FRIEND_TEST(CommunicationInterfaceTest, RQ004_DecodeState_Success);
    FRIEND_TEST(CommunicationInterfaceTest, RQ004_DecodeState_MissingDeviceId);
    FRIEND_TEST(CommunicationInterfaceTest, RQ005_EncryptDecrypt_Success);
};

#endif // COMMUNICATION_INTERFACE_H
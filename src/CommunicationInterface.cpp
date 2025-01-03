#include "CommunicationInterface.h"
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>

// Constructor and Destructor
CommunicationInterface::CommunicationInterface(std::unique_ptr<ISecurity> securityModule)
    : securityModule_(std::move(securityModule)) { 

    // Initialize communication channels of underlying networking platform
}

CommunicationInterface::~CommunicationInterface() {
    // Clean up resources if needed in later implementations
}

// Data Manipulation Methods

/**
 * @brief Encodes a DataPacket::Command object to a JSON string.
 *
 * @param command The DataPacket::Command object to encode.
 * @return JSON string representing the DataPacket::Command.
 */
std::string CommunicationInterface::encodeCommand(const DataPacket::Command& command) {
    nlohmann::ordered_json j;
    j["commandName"] = command.commandName;
    j["speed"] = command.speed;
    j["duration"] = command.duration;
    return j.dump();
}

/**
 * @brief Decodes a JSON string to a DataPacket::State object.
 *
 * @param jsonStr The JSON string to decode.
 * @param state The DataPacket::State object to populate.
 * @return true if decoding is successful, false otherwise.
 */
bool CommunicationInterface::decodeState(const std::string& jsonStr, DataPacket::State& state) {
    try {
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        state.status = j.at("status").get<std::string>();
        state.value = j.at("value").get<int>();
        state.validate();
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Decoding error: " << e.what() << "\n";
        return false;
    }
}

// Public Methods

/**
 * @brief Sends a control command after validating, encoding, and encrypting it.
 *
 * @param command The DataPacket::Command object to send.
 * @return true if sending is successful, false otherwise.
 */
bool CommunicationInterface::sendControlCommand(const DataPacket::Command& command) {
    std::lock_guard<std::mutex> lock(mtx_);
    try {
        command.validate();
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Validation error: " << e.what() << "\n";
        return false;
    }

    std::string encoded = encodeCommand(command);
    if(encoded.empty()) {
        std::cerr << "Encoding failed.\n";
        return false;
    }
    // Logging for demonstration purposes
    std::cout << "Encoded Command to be sent: " << encoded << "\n";
    
    std::string encrypted = securityModule_->encrypt(encoded);
    if(encrypted.empty()) {
        std::cerr << "Encryption failed.\n";
        return false;
    }
    return sendData(encrypted);
}

/**
 * @brief Receives state data, decrypts, decodes, and validates it.
 *
 * @param state The DataPacket::State object to populate with received data.
 * @return true if receiving and processing is successful, false otherwise.
 */
bool CommunicationInterface::receiveState(DataPacket::State& state) {
    std::lock_guard<std::mutex> lock(mtx_);
    std::string encryptedData;
    if (!receiveData(encryptedData)) {
        std::cerr << "Failed to receive data.\n";
        return false;
    }
    std::string decrypted = securityModule_->decrypt(encryptedData);
    if(decrypted.empty()) {
        std::cerr << "Decryption failed.\n";
        return false;
    }
    if(!decodeState(decrypted, state)) {
        std::cerr << "Failed to decode state.\n";
        return false;
    }

    // Invoke callback if set
    if (stateCallback_) {
        stateCallback_(state);
    }

    return true;
}

/**
 * @brief Sets a callback function to handle received states.
 *
 * @param callback A function that takes a const DataPacket::State& as parameter.
 */
void CommunicationInterface::setStateCallback(std::function<void(const DataPacket::State&)> callback) {
    std::lock_guard<std::mutex> lock(mtx_);
    stateCallback_ = callback;
}

// Communication Methods (Platform-Agnostic Placeholder)

/**
 * @brief Placeholder method to simulate sending data.
 *
 * @param data The encrypted data to send.
 * @return true if sending is successful, false otherwise.
 */
bool CommunicationInterface::sendData(const std::string& data) {
    // Placeholder: Simulate sending data over a network
    std::cout << "Sending Encrypted Data: " << data << "\n";
    return true;
}

/**
 * @brief Placeholder method to simulate receiving data.
 *
 * @param data The encrypted data received.
 * @return true if receiving is successful, false otherwise.
 */
bool CommunicationInterface::receiveData(std::string& data) {
    // Simulate receiving encrypted data by encrypting sample plaintext
    std::string samplePlainText = "{\"status\": \"OK\", \"value\": 42}";
    data = securityModule_->encrypt(samplePlainText);
    
    if(data.empty()) {
        std::cerr << "Failed to encrypt sample received data.\n";
        return false;
    }

    std::cout << "Received Encrypted Data: " << data << "\n";
    return true;
}

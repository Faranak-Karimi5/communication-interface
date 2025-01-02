#ifndef COMMUNICATION_INTERFACE_H
#define COMMUNICATION_INTERFACE_H

#include <string>
#include <functional>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>

class CommunicationInterface {
public:
    // Constructor and Destructor
    CommunicationInterface();
    ~CommunicationInterface();

    // Public Methods
    bool sendControlCommand(const nlohmann::json& command);
    bool receiveState(nlohmann::json& state);


private:
    // Encoding and Decoding
    std::string encodeData(const nlohmann::json& data);
    bool decodeData(const std::string& dataStr, nlohmann::json& data);


    // Communication Methods (Platform-Agnostic Placeholder)
    bool sendData(const std::string& data);
    bool receiveData(std::string& data);

    // Encryption-Decryption Methods 
    std::string encryptData(const std::string& plainText);
    std::string decryptData(const std::string& cipherText);

    // Member Variables
    std::mutex mtx_; // For thread safety
    
};

#endif // COMMUNICATION_INTERFACE_H

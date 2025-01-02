#include "CommunicationInterface.h"
#include <iostream>

CommunicationInterface::CommunicationInterface() {
    // Constructor
    std::cout<<"Constructor called"<<std::endl;
}

CommunicationInterface::~CommunicationInterface() {
    // Destructor
    std::cout<<"Destructor called"<<std::endl;
    
}

bool CommunicationInterface::sendControlCommand(const nlohmann::json& command) {
    std::lock_guard<std::mutex> lock(mtx_);
    std::string encoded = encodeData(command);
    //  Encrypt the data before sending it 
    std::string encrypted = encryptData(encoded);
    return sendData(encrypted);
}

bool CommunicationInterface::receiveState(nlohmann::json& state) {
    std::lock_guard<std::mutex> lock(mtx_);
    std::string received;
    if (!receiveData(received)) {
        return false;
    }

    // Decrypt the recived data first
    std::string decrypted = decryptData(received);

    // Extract the state feild from recived JSON
    return decodeData(decrypted, state); 
}


std::string CommunicationInterface::encodeData(const nlohmann::json& data) {
    return data.dump();
}

//
bool CommunicationInterface::decodeData(const std::string& dataStr, nlohmann::json& data) {
    try {
        data = nlohmann::json::parse(dataStr);
        return true;
    } catch (nlohmann::json::parse_error& e) {
        std::cerr << "JSON Parsing Error: " << e.what() << "\n";
        return false;
    }
}

bool CommunicationInterface::sendData(const std::string& data) {
    // Placeholder for sending data over the communication medium
        std::cout << "Sending Data: " << data << "\n";
    return true;
}

bool CommunicationInterface::receiveData(std::string& data) {
    // Assume data is recived
    data = R"({"status": "OK", "value": 42})";
    std::cout << "Received Data: " << data << "\n";
    return true;
}

// Encryption function
std::string CommunicationInterface::encryptData(const std::string& plainText) {
    
    std::string cipherText;
    cipherText = plainText;
    return  cipherText;
}

// Decryption function
std::string CommunicationInterface::decryptData(const std::string& cipherText) {

    std::string plainText;
    plainText = cipherText;
    return plainText;
}

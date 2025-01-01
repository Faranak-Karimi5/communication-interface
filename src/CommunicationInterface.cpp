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
    return sendData(encoded);
}

// Assume data format: JSON
bool CommunicationInterface::receiveState(nlohmann::json& state) {
    std::lock_guard<std::mutex> lock(mtx_);
    std::string received;
    if (!receiveData(received)) {
        return false;
    }

    return decodeData(received, state); 
}


// Assume data format: JSON
std::string CommunicationInterface::encodeData(const nlohmann::json& data) {
    return data.dump();
}

// Assume data format: JSON
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


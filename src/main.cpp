#include "CommunicationInterface.h"
#include "AESCBCSecurity.h"
#include "DataPacket.h"
#include <iostream>
#include <memory>

#include <fstream>
#include <string>
#include <cstdlib>

std::string get_env_var(const std::string& key) {
    char* val = getenv(key.c_str());
    return val == NULL ? std::string("") : std::string(val);
}

std::string read_env_file(const std::string& key) {
    std::ifstream file(".env");
    std::string line;
    while (std::getline(file, line)) {
        if (line.find(key + "=") == 0) {
            return line.substr(key.length() + 1);
        }
    }
    return "";
}


int main() {

    // Get the key from the environment variable (Docker)
    std::string keyEnv = get_env_var("COMM_INTERFACE_KEY");
    // Read the key from ENV File (Direct builds)
    if (keyEnv.empty()) {
        keyEnv = read_env_file("COMM_INTERFACE_KEY");
    }
    
    if (!keyEnv.empty()) {
        std::cout << "COMM_INTERFACE_KEY: " << keyEnv << std::endl;
    } else {
        std::cerr << "COMM_INTERFACE_KEY not found" << std::endl;
        return 1;
    }
    
    // Instantiate the AES-CBC security module with the pre-shared key

    std::unique_ptr<ISecurity> securityModule;
    try {
        securityModule = std::make_unique<AESCBCSecurity>(keyEnv);
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Security module initialization failed: " << e.what() << "\n";
        return 1;
    }

    // Instantiate CommunicationInterface with the security module
    CommunicationInterface comm(std::move(securityModule));

    // Optional: Set a callback for receiving states
    comm.setStateCallback([](const DataPacket::State& state) {
        std::cout << "Callback - Received State:\n";
        std::cout << "Status: " << state.status << "\n";
        std::cout << "Value: " << state.value << "\n";
    });

    // Creating and sending a control command
    DataPacket::Command command;
    command.commandName = "START";
    command.speed = 100;
    command.duration = 60;

    if(comm.sendControlCommand(command)) {
        std::cout << "Control command sent successfully.\n";
    } else {
        std::cerr << "Failed to send control command.\n";
    }

    // Receiving the state
    DataPacket::State state;
    if(comm.receiveState(state)) {
        std::cout << "Received State:\n";
        std::cout << "Status: " << state.status << "\n";
        std::cout << "Value: " << state.value << "\n";
    } else {
        std::cerr << "Failed to receive state.\n";
    }

    return 0;
}

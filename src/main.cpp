#include "CommunicationInterface.h"
#include "AESCBCSecurity.h"
#include "DataPacket.h"
#include <iostream>
#include <memory>



int main() {
    // Pre-shared key (hexadecimal representation)
    std::string keyHex = "00112233445566778899AABBCCDDEEFF"; // 128-bit key

    // Instantiate the AES-CBC security module with the pre-shared key
    std::unique_ptr<ISecurity> securityModule;
    try {
        securityModule = std::make_unique<AESCBCSecurity>(keyHex);
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

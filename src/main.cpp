#include <iostream>
#include "CommunicationInterface.h"

int main() {
    CommunicationInterface comm;

    // Sending a control command
    nlohmann::json command = {
        {"command", "START"},
        {"parameters", {{"VehSpeed", 100}, {"WheelAngle", 60}},
        {"state", "RUNNING"}}
    };
    if(comm.sendControlCommand(command)) {
        std::cout << "Control command sent successfully.\n";
    } else {
        std::cerr << "Failed to send control command.\n";
    }

    // Receiving the state
    nlohmann::json state;
    if(comm.receiveState(state)) {
        std::cout << "Received State is: " << state.dump(4) << "\n";
    } else {
        std::cerr << "Failed to receive state.\n";
    }

    return 0;
}

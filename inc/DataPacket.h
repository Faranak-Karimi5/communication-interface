#ifndef DATA_PACKET_H
#define DATA_PACKET_H

#include <string>
#include <stdexcept>

/**
 * @brief Represents a control command to be sent to the device.
 */
struct Command {
    std::string commandName;
    int speed;
    int duration;

    /**
     * @brief Validates the Command data.
     *
     * @throws std::invalid_argument if any validation fails.
     */
    void validate() const {
        if(commandName.empty()) {
            throw std::invalid_argument("Command name cannot be empty.");
        }
        if(speed < 0 || speed > 1000) { // Example range
            throw std::invalid_argument("Speed must be between 0 and 1000.");
        }
        if(duration <= 0) {
            throw std::invalid_argument("Duration must be positive.");
        }
    }
};

/**
 * @brief Represents the state received from the device.
 */
struct State {
    std::string status;
    int value;

    /**
     * @brief Validates the State data.
     *
     * @throws std::invalid_argument if any validation fails.
     */
    void validate() const {
        if(status.empty()) {
            throw std::invalid_argument("Status cannot be empty.");
        }
        // Additional validations can be added here
    }
};

#endif // DATA_PACKET_H

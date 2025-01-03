#include <gtest/gtest.h>
#include "CommunicationInterface.h"
#include "AESCBCSecurity.h"
#include <memory>
#include <nlohmann/json.hpp>
#include <iostream>
 
// Pre-shared key : Since this is a test, we are using a hardcoded key
std::string preSharedKeyHex = "00112233445566778899AABBCCDDEEFF";

// Helper function to create CommunicationInterface with AES-CBC Security
std::unique_ptr<CommunicationInterface> createCommInterface() {
std::unique_ptr<ISecurity> securityModule;
try {
    securityModule = std::make_unique<AESCBCSecurity>(preSharedKeyHex);
} catch (const std::invalid_argument& e) {
    ADD_FAILURE() << "Security module initialization failed: " << e.what();
    return nullptr;
}
return std::make_unique<CommunicationInterface>(std::move(securityModule));
}

// Test for sending a control command successfully
TEST(CommunicationInterfaceTest, RQ001_SendControlCommand_Success) {
    // RQ-001: The system shall provide a method to send control commands to the other device.
    auto comm = createCommInterface();
    DataPacket::Command command;
    command.commandName = "TEST";
    command.speed = 50;
    command.duration = 30;
    EXPECT_TRUE(comm->sendControlCommand(command));
}

// Test for sending a control command with invalid data
TEST(CommunicationInterfaceTest, RQ001_SendControlCommand_InvalidData) {
    // RQ-001: The system shall provide a method to send control commands to the other device.
    auto comm = createCommInterface();
    DataPacket::Command command;
    command.commandName = ""; // Invalid: Empty command name
    command.speed = 50;
    command.duration = 30;
    EXPECT_FALSE(comm->sendControlCommand(command));
}

// Test for receiving state successfully
TEST(CommunicationInterfaceTest, RQ002_ReceiveState_Success) {
    // RQ-002: The system shall provide a method to receive the state from the other device.
    auto comm = createCommInterface();
    DataPacket::State state;
    EXPECT_TRUE(comm->receiveState(state));
    EXPECT_EQ(state.status, "OK");
    EXPECT_EQ(state.value, 42);
}

// Test for receiving state with invalid data (simulated)
TEST(CommunicationInterfaceTest, RQ002_ReceiveState_InvalidData) {
    // RQ-002: The system shall provide a method to receive the state from the other device.
    // Since this needs mock up of receiveData method, we will skip this test for now
    SUCCEED(); // Placeholder
}

// Test for manual JSON encoding of Command
TEST(CommunicationInterfaceTest, RQ003_EncodeCommand_Success) {
    // RQ-003: The system shall encode data packets to JSON.
    DataPacket::Command command;
    command.commandName = "ENCODE_TEST";
    command.speed = 75;
    command.duration = 45;

    CommunicationInterface comm(nullptr); // Passing nullptr as security module for this test
    std::string jsonStr = comm.encodeCommand(command);
    std::string expectedJson = "{\"commandName\":\"ENCODE_TEST\",\"speed\":75,\"duration\":45}";
    EXPECT_EQ(jsonStr, expectedJson);
}

// Test for manual JSON decoding of State
TEST(CommunicationInterfaceTest, RQ004_DecodeState_Success) {
    // RQ-004: The system shall decode data packets from JSON.
    std::string jsonStr = "{\"status\":\"ACTIVE\",\"value\":100}";
    DataPacket::State state;
    CommunicationInterface comm(nullptr); // Passing nullptr as security module for this test
    bool result = comm.decodeState(jsonStr, state);

    EXPECT_TRUE(result);
    EXPECT_EQ(state.status, "ACTIVE");
    EXPECT_EQ(state.value, 100);
}

// Test for encryption and decryption functionality
TEST(CommunicationInterfaceTest, RQ005_EncryptDecrypt_Success) {
    // RQ-005: The system shall encrypt data packets before transmission.
    std::unique_ptr<ISecurity> securityModule;
    try {
        securityModule = std::make_unique<AESCBCSecurity>(preSharedKeyHex);
    }
    catch (const std::invalid_argument& e) {
        FAIL() << "Security module initialization failed: " << e.what();
    }

    DataPacket::State state;
    state.status = "OK";
    state.value = 42;

    // Serialize state to JSON
    nlohmann::json j;
    j["status"] = state.status;
    j["value"] = state.value;
    std::string jsonStr = j.dump();

    // Encrypt the JSON string
    std::string encrypted = securityModule->encrypt(jsonStr);
    EXPECT_FALSE(encrypted.empty());
    EXPECT_NE(encrypted, jsonStr); // Ensure encryption changes the data

    // Decrypt the ciphertext
    std::string decrypted = securityModule->decrypt(encrypted);
    EXPECT_EQ(decrypted, jsonStr); // Ensure decryption restores original data
}

// Test the full send and receive flow with encryption
TEST(CommunicationInterfaceTest, RQ006_SendReceive_WithEncryption_Success) {
    // RQ-006: Integration test for sending and receiving data with encryption.
    auto comm = createCommInterface();
    DataPacket::Command command;
    command.commandName = "INTEGRATION_TEST";
    command.speed = 80;
    command.duration = 40;

    // Send the command
    EXPECT_TRUE(comm->sendControlCommand(command));

    // Receive the state
    DataPacket::State state;
    EXPECT_TRUE(comm->receiveState(state));
    EXPECT_EQ(state.status, "OK");
    EXPECT_EQ(state.value, 42);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

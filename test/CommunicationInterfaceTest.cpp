#include <gtest/gtest.h>
#include "CommunicationInterface.h"
#include <nlohmann/json.hpp>


TEST(SendControlCommandTest, SendJSONFormatCommand) {
    CommunicationInterface comm;
    nlohmann::json command = { {"command", "TEST"} };
    // Non-fatal continue the test if it fails
    EXPECT_TRUE(comm.sendControlCommand(command));
}

int main(int argc, char **argv) {
    // GTest Setup and Initalizations
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

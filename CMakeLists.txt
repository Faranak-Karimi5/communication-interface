cmake_minimum_required(VERSION 3.10)

project(CommunicationInterface)

include_directories(${PROJECT_SOURCE_DIR}/inc)

# Copy env file to build directory
file(COPY ${PROJECT_SOURCE_DIR}/.env DESTINATION ${PROJECT_SOURCE_DIR}/build)# JSON library
find_package(nlohmann_json 3.2.0 REQUIRED)

# crypto++
add_subdirectory(${PROJECT_SOURCE_DIR}/cryptopp-cmake)

# Google Test
find_package(GTest REQUIRED)

# Main executable
add_executable(communication_interface
    src/main.cpp
    src/CommunicationInterface.cpp
    src/AESCBCSecurity.cpp
)

target_link_libraries(communication_interface
    PRIVATE
    nlohmann_json::nlohmann_json
    cryptopp::cryptopp
)

# Create executable service for test suite 
add_executable(runTests 
    test/CommunicationInterfaceTest.cpp 
    src/CommunicationInterface.cpp
    src/AESCBCSecurity.cpp
)

target_link_libraries(runTests 
    PRIVATE 
    GTest::GTest
    GTest::Main
    nlohmann_json::nlohmann_json
    cryptopp::cryptopp
)

enable_testing()

add_test(NAME CommunicationInterfaceTests COMMAND runTests)

cmake_minimum_required(VERSION 3.10)

project(CommunicationInterface)

# Use C++11 Standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set build and header directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
include_directories(${PROJECT_SOURCE_DIR}/inc)

# Packages for parsing Json and test
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(GTest REQUIRED)


add_subdirectory(/app/cryptopp-cmake)


#  Create executable service for main application 
add_executable(communication_interface 
    src/main.cpp 
    src/CommunicationInterface.cpp
    src/AESCBCSecurity.cpp
)

# Link the nlohmann_json for JSON parsing and Crypto++ for encrypt/decrypt
target_link_libraries(communication_interface PRIVATE 
    nlohmann_json::nlohmann_json 
    cryptopp::cryptopp
)
# Create executable service for test suite 
add_executable(runTests 
    test/CommunicationInterfaceTest.cpp 
    src/CommunicationInterface.cpp
    src/AESCBCSecurity.cpp
)

include_directories(${CRYPTOPP_INCLUDE_DIR})
include_directories(${GTEST_INCLUDE_DIRS})


# Link the Google Test, nlohmann_json, and Crypto++ libraries to the test executable
target_link_libraries(runTests PRIVATE 
    GTest::GTest 
    GTest::Main 
    nlohmann_json::nlohmann_json
    cryptopp::cryptopp
)

enable_testing()

add_test(NAME CommunicationInterfaceTests COMMAND runTests)

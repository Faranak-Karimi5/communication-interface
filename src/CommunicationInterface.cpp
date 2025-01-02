#include "CommunicationInterface.h"
#include <iostream>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <sstream>
#include <cstring>


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
    // Assume encrypted data is recived from a device 
    std::string PlainData = R"({"status": "OK", "value": 42})";
    data = encryptData(PlainData);
    std::cout << "Received Data (Encrypted): " << data << "\n";
    return true;
}

// Encryption function
std::string CommunicationInterface::encryptData(const std::string& plainText) {
    
    using namespace CryptoPP;
    
    AutoSeededRandomPool prng;
    
    byte key[AES::DEFAULT_KEYLENGTH];
    byte iv[AES::BLOCKSIZE];
    
    prng.GenerateBlock(key, sizeof(key));
    prng.GenerateBlock(iv, sizeof(iv));
    
    std::string cipherText;
    
    try {
        CBC_Mode< AES >::Encryption encryption;
        encryption.SetKeyWithIV(key, sizeof(key), iv);
        
        StringSource ss(plainText, true,
            new StreamTransformationFilter(encryption,
                new StringSink(cipherText)
            )
        );
    }
    catch (const Exception& e) {
        std::cerr << "Encryption error: " << e.what() << std::endl;
        return "";
    }
    
    // Prepend IV for use in decryption
    std::string ivStr(reinterpret_cast<char*>(iv), sizeof(iv));
    return ivStr + cipherText;
}

// Decryption function
std::string CommunicationInterface::decryptData(const std::string& cipherText) {

    using namespace CryptoPP;
    
    if(cipherText.size() < AES::BLOCKSIZE) {
        std::cerr << "Cipher text too short to contain IV." << std::endl;
        return "";
    }
    
    const byte* iv = reinterpret_cast<const byte*>(cipherText.data());
    const byte* actualCipherText = reinterpret_cast<const byte*>(cipherText.data() + AES::BLOCKSIZE);
    size_t cipherSize = cipherText.size() - AES::BLOCKSIZE;
    byte key[AES::DEFAULT_KEYLENGTH];
    
    // Retrieve the key from a secure source; for example purposes, using a hardcoded key
    // WARNING: Hardcoding keys is insecure and only for demonstration purposes
    std::memcpy(key, "ThisIsASecretKey", 16);
    
    std::string plainText;
    
    try {
        CBC_Mode< AES >::Decryption decryption;
        decryption.SetKeyWithIV(key, sizeof(key), iv);
        
        StringSource ss(reinterpret_cast<const byte*>(actualCipherText), cipherSize, true,
            new StreamTransformationFilter(decryption,
                new StringSink(plainText)
            )
        );
    }
    catch (const Exception& e) {
        std::cerr << "Decryption error: " << e.what() << std::endl;
        return "";
    }
    
    return plainText;
}

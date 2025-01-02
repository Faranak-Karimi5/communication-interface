#include "AESCBCSecurity.h"
#include <cryptopp/cryptlib.h>
#include <cryptopp/osrng.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/hex.h>
#include <iostream>
#include <cstring>

AESCBCSecurity::AESCBCSecurity(const std::string& keyHex) {
    // Decode the hexadecimal key string to bytes
    CryptoPP::HexDecoder decoder;
    decoder.Put(reinterpret_cast<const unsigned char*>(keyHex.data()), keyHex.size());
    decoder.MessageEnd();

    size_t keyLen = decoder.MaxRetrievable();
    if (keyLen != CryptoPP::AES::DEFAULT_KEYLENGTH) {
        throw std::invalid_argument("Invalid key length. Expected " + std::to_string(CryptoPP::AES::DEFAULT_KEYLENGTH) + " bytes.");
    }

    key_.Resize(CryptoPP::AES::DEFAULT_KEYLENGTH);
    decoder.Get(key_, key_.size());
}

AESCBCSecurity::~AESCBCSecurity() {
    // Zero out the key memory for security
    CryptoPP::ZeroMemory(key_.BytePtr(), key_.size());
}

std::string AESCBCSecurity::encrypt(const std::string& plainText) {
    using namespace CryptoPP;

    AutoSeededRandomPool prng;

    byte iv[AES::BLOCKSIZE];
    prng.GenerateBlock(iv, sizeof(iv));

    std::string cipherText;

    try {
        CBC_Mode<AES>::Encryption encryption;
        encryption.SetKeyWithIV(key_, key_.size(), iv);

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

    // Prepend IV to ciphertext
    std::string ivStr(reinterpret_cast<char*>(iv), AES::BLOCKSIZE);
    return ivStr + cipherText;
}

std::string AESCBCSecurity::decrypt(const std::string& cipherText) {
    using namespace CryptoPP;

    if (cipherText.size() < AES::BLOCKSIZE) {
        std::cerr << "Cipher text too short to contain IV." << std::endl;
        return "";
    }

    // Extract IV
    const byte* iv = reinterpret_cast<const byte*>(cipherText.data());
    const byte* actualCipherText = reinterpret_cast<const byte*>(cipherText.data() + AES::BLOCKSIZE);
    size_t cipherSize = cipherText.size() - AES::BLOCKSIZE;

    std::string plainText;

    try {
        CBC_Mode<AES>::Decryption decryption;
        decryption.SetKeyWithIV(key_, key_.size(), iv);

        StringSource ss(reinterpret_cast<const char*>(actualCipherText), cipherSize, true,
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

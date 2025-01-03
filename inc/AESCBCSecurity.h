// include/AESCBCSecurity.h
#ifndef AESCBCC_SECURITY_H
#define AESCBCC_SECURITY_H

#include "ISecurity.h"
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <string>

/**
 * @brief AES-CBC encryption and decryption implementation.
 */
class AESCBCSecurity : public ISecurity {
public:
 
    AESCBCSecurity(const std::string& keyHex);
    ~AESCBCSecurity();


    std::string encrypt(const std::string& plainText) override;

    std::string decrypt(const std::string& cipherText) override;

private:
    CryptoPP::SecByteBlock key_; // Encryption key, assigned at runtime
};

#endif // AESCBCC_SECURITY_H

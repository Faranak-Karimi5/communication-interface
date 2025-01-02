// include/ISecurity.h
#ifndef ISECURITY_H
#define ISECURITY_H

#include <string>

/**
 * @brief Interface for security operations.
 *
 * Defines the contract for encryption and decryption strategies.
 */
class ISecurity {
public:
    virtual ~ISecurity() {}

    /**
     * @brief Encrypts the given plaintext.
     *
     * @param plainText The data to encrypt.
     * @return The encrypted data.
     */
    virtual std::string encrypt(const std::string& plainText) = 0;

    /**
     * @brief Decrypts the given ciphertext.
     *
     * @param cipherText The data to decrypt.
     * @return The decrypted data.
     */
    virtual std::string decrypt(const std::string& cipherText) = 0;
};

#endif // ISECURITY_H

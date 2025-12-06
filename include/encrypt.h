/**
 * @file encrypt.h
 * @brief Encrypt plaintext of password or decrypt ciphertext of password
 * @details Handles encrypting or decrypting of password
 */

#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <sodium.h>
#include <stdbool.h>


/**
 * @brief Limit length of plaintext and ciphertext 
 */
#define MAX_PLAINTEXT_LEN 256
#define MAX_CIPHERTEXT_LEN (MAX_PLAINTEXT_LEN + crypto_aead_xchacha20poly1305_ietf_ABYTES)

/**
 * @brief Error messages for encrypting.
 */
#define ERR_INIT "[Program error] INFO: Sodium initalization failed.\n"
#define ERR_MASTERKEY_DERIVE "[Program error] INFO: Master key derivation failed.\n"
#define ERR_PASSWORD_LENGTH "[Program error] INFO: Password is too long.\n"
#define ERR_ENCRYPT "[Program error] INFO: Encrypt password failed.\n"
#define ERR_DECRYPT "[Usage Error] INFO: Incorrect master key or corrupted data.\n"


/**
 * @brief Encrypted information.
 */
typedef struct
{
    // Argond2id salt
    unsigned char masterKeySalt[crypto_pwhash_SALTBYTES];

    // Nonce
    unsigned char nonce[crypto_aead_xchacha20poly1305_ietf_NPUBBYTES];

    // Encrypted password
    unsigned char encryptedPassword[MAX_CIPHERTEXT_LEN];

    // The length of encrypted password
    unsigned long long encryptedPasswordLength;
} EncryptedData;


/** 
* @brief Initialize sodium engine.
*
* @return true if initialize sodium successfully.
* @return false if initialize sodium failed.
*/
bool initSodium(void);

/**
 * @brief Encrypt password using master key.
 * 
 * @param masterKey Master key.
 * @param password Password need to be encrypt.
 * @param data Encrypted information.
 * @return true if encrypt successfully.
 * @return false if encrypt failed.
 */
bool encryptPassword(const char *masterKey, const char *password, EncryptedData *data);

/**
 * @brief Decrypt encrypted password using master key.
 * 
 * @param masterKey Master key.
 * @param data Encrypted information.
 * @param plaintext A pointer to the location where the plaintext was output.
 * @return true if decrypt successfully.
 * @return false if decrypt failed.
 */
bool decryptEncryptedPassword(const char *masterKey, EncryptedData *data, char *plaintext);

#endif // ENCRYPT_H
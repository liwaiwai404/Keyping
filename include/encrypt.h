#ifndef ENCRYPT_H
#define ENCRYPT_H

// Include header file
#include <sodium.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>


// Macro definition
// Max length of plaintext
#define MAX_PLAINTEXT_LEN 256
// Max length of ciphertext
#define MAX_CIPHERTEXT_LEN (MAX_PLAINTEXT_LEN + crypto_aead_xchacha20poly1305_ietf_ABYTES)

// Error infomation
#define ERR_INIT "[Program error] INFO: Sodium initalization failed.\n"
#define ERR_MASTERKEY_DERIVE "[Program error] INFO: Master key derivation failed.\n"
#define ERR_PASSWORD_LENGTH "[Program error] INFO: Password is too long.\n"
#define ERR_ENCRYPT "[Program error] INFO: Encrypt password failed.\n"
#define ERR_DECRYPT "[Usage Error] INFO: Incorrect master password or corrupted data.\n"


// Structure of data need to store
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


// Initialize sodium engine
// Return true if success
// else return false
bool initSodium(void);

// Encrypt password using master key and store into data
bool encryptPassword(const char *masterKey, const char *password, EncryptedData *data);

// Decrypt encrypted password
bool decryptEncryptedPassword(const char *masterKey, EncryptedData *data, char *plaintext);

#endif
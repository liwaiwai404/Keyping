#include "encrypt.h"

/** 
* @brief Initialize sodium engine
* @return true success, false failed
*/
bool initSodium(void)
{
    // Initialize failed
    if (sodium_init() == -1)
    {
        fprintf(stderr, ERR_INIT);
        return false;
    }

    // Initialize success 
    return true;
}

/**
 * @brief Encrypt password using master key
 * @param masterKey Master key
 * @param password Password need to be encrypt
 * @param data Place to store encrypted info
 * @return true success, false failed
 */
bool encryptPassword(const char *masterKey, const char *password, EncryptedData *data)
{
    // Variable to store encrypted master key
    unsigned char encryptedMasterKey[crypto_aead_xchacha20poly1305_ietf_KEYBYTES];

    // Generate random salt and nonce
    randombytes_buf(data->masterKeySalt, sizeof(data->masterKeySalt));
    randombytes_buf(data->nonce, sizeof(data->nonce));

    // Generate encrypted master key
    if (crypto_pwhash(encryptedMasterKey, sizeof(encryptedMasterKey), masterKey, strlen(masterKey), data->masterKeySalt, 
                      crypto_pwhash_OPSLIMIT_MODERATE, crypto_pwhash_MEMLIMIT_MODERATE, crypto_pwhash_ALG_DEFAULT) != 0)
    {
        fprintf(stderr, ERR_MASTERKEY_DERIVE);
        return false;
    }

    // Encrypted password
    // Get plaintext and length of password
    unsigned char *plaintext = (unsigned char *)password;
    size_t plaintextLength = strlen(password);

    // Check the length of password
    if (plaintextLength >= MAX_PLAINTEXT_LEN)
    {
        fprintf(stderr, ERR_PASSWORD_LENGTH);
        // Clean the data of encrypted master key in memory
        sodium_memzero(encryptedMasterKey, sizeof(encryptedMasterKey));
        return false;
    }

    // Encrypt password
    if (crypto_aead_xchacha20poly1305_ietf_encrypt(data->encryptedPassword, &data->encryptedPasswordLength, plaintext, plaintextLength,
                                                   NULL, 0, NULL, data->nonce, encryptedMasterKey) != 0)
    {
        fprintf(stderr, ERR_ENCRYPT);
        // Clean the data of encrypted master key in memory
        sodium_memzero(encryptedMasterKey, sizeof(encryptedMasterKey));
        return false;
    }

    // Clean the data of encrypted master key in memory
    sodium_memzero(encryptedMasterKey, sizeof(encryptedMasterKey));
    return true;
}

/**
 * @brief Decrypt encrypted password using master key
 * @param masterKey Master key
 * @param data Place to store encrypted info
 * @param password Place to put decrypted infomation
 * @param passwordLength Length of password
 * @return true: decrypt success, false failed
 */
bool decryptEncryptedPassword(const char *masterKey, EncryptedData *data, char *plaintext)
{
    // Variable to store encrypted master key
    unsigned char encryptedMasterKey[crypto_aead_xchacha20poly1305_ietf_KEYBYTES];
    // Length of plaintext of password
    unsigned long long plaintextLength; 

    // Generate encrypted master key
    if (crypto_pwhash(encryptedMasterKey, sizeof(encryptedMasterKey), masterKey, strlen(masterKey), data->masterKeySalt, 
                      crypto_pwhash_OPSLIMIT_MODERATE, crypto_pwhash_MEMLIMIT_MODERATE, crypto_pwhash_ALG_DEFAULT) != 0) 
    {
        fprintf(stderr, ERR_MASTERKEY_DERIVE);
        return false;
    }

    // Decrypt ciphertext of password
    if (crypto_aead_xchacha20poly1305_ietf_decrypt((unsigned char *)plaintext, &plaintextLength, NULL,
                                              data->encryptedPassword, data->encryptedPasswordLength, NULL, 0, 
                                              data->nonce, encryptedMasterKey) != 0) 
    {
        fprintf(stderr, ERR_DECRYPT);

        // Clean the data of encrypted master key in memory
        sodium_memzero(encryptedMasterKey, sizeof(encryptedMasterKey));
        return false;
    }

    // Null-terminate the plaintext string
    plaintext[plaintextLength] = '\0';

    // Clean the data of encrypted master key in memory
    sodium_memzero(encryptedMasterKey, sizeof(encryptedMasterKey)); 
    return true;
}
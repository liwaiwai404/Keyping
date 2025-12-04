#include <stdio.h>

#include "cl_parser.h"
#include "encrypt.h"

int main(int argc, char **argv)
{
    // Check CLI
    if (!checkFormat(argc, argv))
        return 1;

    //char *flag = argv[1];

    // Get mode
    //enum Mode mode = getMode(flag[1]);

    // Initialize sodium engine
    if(!initSodium())
        return 2;
    
    const char *masterKey = "123456";
    const char *password = "wasd1234!@#$";
    EncryptedData data;
    char plaintext[MAX_PLAINTEXT_LEN];

    printf("Master key: %s\n", masterKey);

    encryptPassword(masterKey, password, &data);
    decryptEncryptedPassword(masterKey, &data, plaintext);

    printf("Original: %s\n", password);
    printf("Chipertext: %s Length: %lld\n", data.encryptedPassword, data.encryptedPasswordLength);

    printf("Decrypt: %s\n", plaintext);




    return 0;
}
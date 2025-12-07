#include "handler.h"
#include "encrypt.h"
#include <sodium.h>
#include <stdio.h>
#include "storage.h"
#include "utils.h"
 

bool handleAdd(PasswordInfo *info, sqlite3 *db)
{
    EncryptedData encryptedData;

    // Encrypt data
    if (!encryptPassword(info->masterKey, info->password, &encryptedData))
        return false;
    
    // Storage data
    if (!addAccount(db, info, &encryptedData))
        return false;

    // Clean memory
    sodium_memzero(&encryptedData, sizeof(encryptedData));
    
    return true;
}

bool handleList(sqlite3 *db, const char *keyword)
{
    // Print header
    printHeader();
    
    // Get data and print every row of data
    if (!listAccounts(db, keyword, printDataByRow, NULL))
        return false;

    // Print footer
    printFooter();
    
    return true;
}

bool handleQuery(PasswordInfo *info, sqlite3 *db)
{
    EncryptedData encryptedData;

    // Get encrypted data
    if (!queryAccount(db, info, &encryptedData))
        return false;

    // Decrypt ciphertext
    char plaintext[MAX_PLAINTEXT_LEN];
    if (!decryptEncryptedPassword(info->masterKey, &encryptedData, plaintext))
        return false;

    // Copy to clipboard
    copyToClipboard(plaintext);

    return true;
}

bool handleModify(PasswordInfo *info, sqlite3 *db)
{
    EncryptedData encryptedData;
    
    // Encrypt data
    if (!encryptPassword(info->masterKey, info->password, &encryptedData))
        return false;
    
    // Update data
    if (!updateAccount(db, info, &encryptedData))
        return false;
               
    // Clean memory
    sodium_memzero(&encryptedData, sizeof(encryptedData));
    
    return true;
}

bool handlDelete(PasswordInfo *info, sqlite3 *db)
{
    if (!deleteAccount(db, info))
        return false;
    
    return true;
}
#include <stdio.h>
#include "cl_parser.h"
#include "cl_interface.h"
#include "encrypt.h"
#include "storage.h"

/**
 * @brief Handl add operation
 * 
 * @param info Password information
 * @param db Database
 */
bool handleAdd(PasswordInfo *info, sqlite3 *db);

/**
 * @brief Display the result of list mode
 * 
 * @param db Database to handle
 * @param keyword Pattern keyword
 */
bool showList(sqlite3 *db, const char *keyword);

/**
 * @brief Handle query password operation
 * 
 * @param info Password information
 * @param db Database
 */
bool handleQuery(PasswordInfo *info, sqlite3 *db);

/**
 * @brief Modify information by description and username
 * 
 * @param info Provide description and username
 * @param db Database
 */
bool handleModify(PasswordInfo *info, sqlite3 *db);

/**
 * @brief Delete a account by desc and username
 * 
 * @param info Provide description and username
 * @param db Database
 */
bool handlDelete(PasswordInfo *info, sqlite3 *db);

/**
 * @brief Program entry point
 */
int main(int argc, char **argv)
{
    // Check CLI
    if (!checkFormat(argc, argv))
        return 1;

    // Get mode
    char *flag = argv[1];
    enum Mode mode = getMode(flag[1]);

    // Get data by mode
    PasswordInfo info;
    if (mode != LIST)
    {
        if (!getPasswordInfo(&info, mode))
            return 2;
    }
    
    // Init database
    sqlite3 *db;
    if (!initDatabase("keying.db", &db))
        return 3;

    // Handle
    switch (mode)
    {
    case ADD:
        if (!handleAdd(&info, db))
            return 4;
        printf("Add account successfully!\n");
        break;

    case LIST:
        char *keyword = NULL;
        // Keyword exist
        if (argc == 3)
            keyword = argv[2];
        if(!showList(db, keyword))
            return 5;
        break;

    case QUERY:
        if (!handleQuery(&info, db))
            return 6;
        printf("Query password successfully!\n");
        break;

    case MODIFY:
        if (!handleModify(&info, db))
            return 7;
        printf("Modify password successfully!\n");
        break;
        
    case DELETE:
        if (!handlDelete(&info, db))
            return 8;
        printf("Delete account successfully!\n");
        break;
    default:
        break;
    }

    // Clean password infomation data
    sodium_memzero(&info, sizeof(info));

    // Close database
    closeDatabase(db);
    return 0;
}

bool handleAdd(PasswordInfo *info, sqlite3 *db)
{
    // Init sodium
    EncryptedData encryptedData;
    if (!initSodium())
        return false;

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

bool showList(sqlite3 *db, const char *keyword)
{
    // Print header
    printf("+---------------------------------------------+\n");
    printf("|                Account list                 |\n");
    printf("+----------------------+----------------------+\n");
    printf("|          Description |             Username |\n");
    printf("+----------------------+----------------------+\n");
    
    // Get data and print every row of data
    if (!listAccounts(db, keyword, printDataByRow, NULL)) {
        return false;
    }

    printf("+----------------------+----------------------+\n");

    return true;
}

bool handleQuery(PasswordInfo *info, sqlite3 *db)
{
    // Init sodium
    EncryptedData encryptedData;
    if (!initSodium())
        return false;

    // Get encrypted data
    if (!queryAccount(db, info, &encryptedData))
        return false;

    // Decrypt ciphertext
    char plaintext[MAX_PLAINTEXT_LEN];
    if (!decryptEncryptedPassword(info->masterKey, &encryptedData, plaintext))
        return false;

    printf("%s\n", plaintext);

    return true;
}

bool handleModify(PasswordInfo *info, sqlite3 *db)
{
    // Init sodium
    EncryptedData encryptedData;
    if (!initSodium())
        return false;
    
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
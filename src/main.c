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
        break;
    case LIST:
        char *keyword = NULL;
        if (argc == 3)
            keyword = argv[2];
        showList(db, keyword);
        break;
    default:
        break;
    }

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
    printf("+----------------------+----------------------+\n");
    
    // Get data and print every row of data
    if (!listAccounts(db, keyword, printDataByRow, NULL)) {
        return false;
    }

    printf("+----------------------+----------------------+\n");

    return true;
}
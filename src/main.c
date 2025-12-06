#include "cl_interface.h"
#include "cl_parser.h"
#include "encrypt.h"
#include "handler.h"
#include <sqlite3.h>
#include <stdio.h>
#include "storage.h"


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
    if (mode == 0)
        return 1;

    // Get data by mode
    PasswordInfo info;
    if (mode != LIST)
    {
        if (!getPasswordInfo(&info, mode))
            return 2;
    }
    
    // Init database
    sqlite3 *db;
    if (!initDatabase("keyping.db", &db))
        return 3;

    // Init sodium engine
    if (!initSodium())
    {
        closeDatabase(db);
        return 4;
    }
        

    // Handle
    short exitCode = 0;
    switch (mode)
    {
    case ADD:
        if (!handleAdd(&info, db))
            exitCode = 5;
        break;

    case LIST:
        char *keyword = NULL;
        // Keyword exist
        if (argc == 3)
            keyword = argv[2];
        if(!handleList(db, keyword))
            exitCode = 6;
        break;

    case QUERY:
        if (!handleQuery(&info, db))
            exitCode = 6;
        break;

    case MODIFY:
        if (!handleModify(&info, db))
            exitCode = 7;
        break;
        
    case DELETE:
        if (!handlDelete(&info, db))
            exitCode = 8;
        break;
    default:
        break;
    }

    // Print success information
    if (exitCode == 0)
        printSuccessInfo(mode);

    // Clean password infomation data
    sodium_memzero(&info, sizeof(info));

    // Close database
    closeDatabase(db);
    return exitCode;
}


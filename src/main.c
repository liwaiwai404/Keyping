#include "cl_interface.h"
#include "cl_parser.h"
#include "encrypt.h"
#include "handler.h"
#include <sqlite3.h>
#include <stdio.h>
#include "storage.h"

/**
 * @brief Exit code
 */
#define EXIT_FORMAT_ERR 1
#define EXIT_GETPWINFO_ERR 2
#define EXIT_DB_INIT 3
#define EXIT_SODIUM_INIT 4
#define EXIT_ADD_ERR 5
#define EXIT_LIST_ERR 6
#define EXIT_QUERY_ERR 7
#define EXIT_MODIFY_ERR 8
#define EXIT_DELETE_ERR 9


/**
 * @brief Program entry point
 */
int main(int argc, char **argv)
{
    // Check CLI
    if (!checkFormat(argc, argv))
        return EXIT_FORMAT_ERR;

    // Get mode
    char *flag = argv[1];
    enum Mode mode = getMode(flag[1]);
    if (mode == 0)
        return EXIT_FORMAT_ERR;

    // Get data by mode
    PasswordInfo info;
    if (mode != LIST)
    {
        if (!getPasswordInfo(&info, mode))
            return EXIT_GETPWINFO_ERR;
    }
    
    // Init database
    sqlite3 *db;
    if (!initDatabase("keyping.db", &db))
        return EXIT_DB_INIT;

    // Init sodium engine
    if (!initSodium())
    {
        closeDatabase(db);
        return EXIT_SODIUM_INIT;
    }
        

    // Handle
    short exitCode = 0;
    switch (mode)
    {
    case ADD:
        if (!handleAdd(&info, db))
            exitCode = EXIT_ADD_ERR;
        break;

    case LIST:
        char *keyword = NULL;
        // Keyword exist
        if (argc == 3)
            keyword = argv[2];
        if(!handleList(db, keyword))
            exitCode = EXIT_LIST_ERR;
        break;

    case QUERY:
        if (!handleQuery(&info, db))
            exitCode = EXIT_QUERY_ERR;
        break;

    case MODIFY:
        if (!handleModify(&info, db))
            exitCode = EXIT_MODIFY_ERR;
        break;
        
    case DELETE:
        if (!handlDelete(&info, db))
            exitCode = EXIT_DELETE_ERR;
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


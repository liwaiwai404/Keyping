#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "cl_interface.h" 
#include <sqlite3.h>
#include <stdbool.h>


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
bool handleList(sqlite3 *db, const char *keyword);

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

#endif // OPERATIONS_H
/**
 * @file storage.h
 * @brief Database operations for Keyping
 */

#ifndef STORAGE_H
#define STORAGE_H

#include <sqlite3.h>
#include <stdbool.h>
#include "encrypt.h"       
#include "cl_interface.h"

/**
 * @brief Error information
 */
#define ERR_OPEN_DB "[DB error] INFO: Open database failed. %s\n"
#define ERR_SQL "[DB error] INFO: SQL error. %s\n"
#define ERR_PREPARE "[DB error] INFO: Prepare failed. %s\n"
#define ERR_INSERT "[DB error] INFO: Insert failed. %s\n"
#define ERR_LIST "[DB error] INFO: List failed. %s\n"


/**
 * @brief Initialize database connection and ensure table exists.
 * 
 * @param dbFilename Path to the database file (e.g., "keyping.db").
 * @param db Output pointer to the sqlite3 connection handle.
 * @return true if success, false otherwise.
 */
bool initDatabase(const char *dbFilename, sqlite3 **db);

/**
 * @brief Close database connection.
 * 
 * @param db The sqlite3 connection handle.
 */
void closeDatabase(sqlite3 *db);

/**
 * @brief Add a new account entry to the database.
 * 
 * @param db Database handle.
 * @param info Plaintext info (username, description).
 * @param enc Encrypted data (salt, nonce, ciphertext).
 * @return true if success, false otherwise.
 */
bool addAccount(sqlite3 *db, const PasswordInfo *info, const EncryptedData *enc);

/**
 * @brief Retrieve an account entry by description.
 * 
 * @param db Database handle.
 * @param description The unique description to search for.
 * @param info Output: will be filled with username (and description).
 * @param enc Output: will be filled with encrypted data (salt, nonce, ciphertext).
 * @return true if found, false if not found or error.
 */
//bool queryAccount(sqlite3 *db, const char *description, PasswordInfo *info, EncryptedData *enc);

/**
 * @brief Define callback function
 * This function is called once every time a row of data is retrieved from the database.
 * 
 * @param description Description found
 * @param username Username found
 * @param user_data 用户传递的额外参数
 */
typedef void (*ListCallback)(const char *description, const char *username, void *user_data);

/**
 * @brief Search or list accounts.
 * 
 * @param db Database handle.
 * @param searchQuery Keyword to search. If NULL or empty, list all.
 * @param callback The function to call for each found entry.
 * @param user_data Extra data passed to the callback.
 * @return true if query executed successfully.
 */
bool listAccounts(sqlite3 *db, const char *searchQuery, ListCallback callback, void *user_data);

#endif // STORAGE_H
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
#define ERR_QUERY "[DB Error] INFO: Query failed. %s\n"
#define ERR_UPDATE "[DB error] INFO: Update failed. %s\n"
#define ERR_DELETE "[DB error] INFO: Delete failed. %s\n"


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
 * @param info Provide description and username
 * @param enc Output: will be filled with encrypted data (salt, nonce, ciphertext).
 * @return true if found, false if not found or error.
 */
bool queryAccount(sqlite3 *db, const PasswordInfo *info, EncryptedData *encData);

/**
 * @brief Define callback function
 * This function is called once every time a row of data is retrieved from the database.
 * 
 * @param description Description found
 * @param username Username found
 * @param user_data Additional data
 */
typedef void (*ListCallback)(const char *description, const char *username, void *user_data);

/**
 * @brief List all accounts or accounts by keyword.
 * 
 * @param db Database handle.
 * @param searchQuery Keyword to search. If NULL or empty, list all.
 * @param callback The function to call for each found entry.
 * @param user_data Extra data passed to the callback.
 * @return true if query executed successfully.
 */
bool listAccounts(sqlite3 *db, const char *searchQuery, ListCallback callback, void *user_data);

/**
 * @brief Update account
 * 
 * @param db Database handle.
 * @param info Provide description and username
 * @param enc Provide new encrypted data (salt, nonce, ciphertext).
 */
bool updateAccount(sqlite3 *db, const PasswordInfo *info, const EncryptedData *enc);

/**
 * @brief Delete account
 * 
 * @param db Database handle.
 * @param info Provide description and username
 */
bool deleteAccount(sqlite3 *db, const PasswordInfo *info);

#endif // STORAGE_H
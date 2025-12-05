#include "storage.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// SQL to create the table
static const char *CREATE_TABLE_SQL = 
    "CREATE TABLE IF NOT EXISTS accounts ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "description TEXT NOT NULL UNIQUE, "
    "username TEXT, "
    "salt BLOB NOT NULL, "
    "nonce BLOB NOT NULL, "
    "ciphertext BLOB NOT NULL, "
    "ciphertext_len INTEGER NOT NULL"
    ");";


bool initDatabase(const char *dbFilename, sqlite3 **db)
{
    // Open database
    if (sqlite3_open(dbFilename, db) != SQLITE_OK)
    {
        fprintf(stderr, ERR_OPEN_DB, sqlite3_errmsg(*db));
        return false;
    }

    // Create table
    char *errMsg = NULL;
    if (sqlite3_exec(*db, CREATE_TABLE_SQL, 0, 0, &errMsg) != SQLITE_OK)
    {
        fprintf(stderr, ERR_SQL, errMsg);
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

void closeDatabase(sqlite3 *db)
{
    if (db)
        sqlite3_close(db);
}

bool addAccount(sqlite3 *db, const PasswordInfo *info, const EncryptedData *enc)
{
    // SQL template
    const char *sql = "INSERT INTO accounts (description, username, salt, nonce, ciphertext, ciphertext_len) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    bool result = false;

    // Prepare
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, ERR_PREPARE, sqlite3_errmsg(db));
        return false;
    }

    // Bind text
    sqlite3_bind_text(stmt, 1, info->description, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, info->username, -1, SQLITE_STATIC);

    // Bind blob
    sqlite3_bind_blob(stmt, 3, enc->masterKeySalt, sizeof(enc->masterKeySalt), SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 4, enc->nonce, sizeof(enc->nonce), SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 5, enc->encryptedPassword, enc->encryptedPasswordLength, SQLITE_STATIC);

    // Bind integer
    sqlite3_bind_int64(stmt, 6, (sqlite_int64)enc->encryptedPasswordLength);

    // Step execute
    if (sqlite3_step(stmt) == SQLITE_DONE)
        result = true;
    else
        fprintf(stderr, ERR_INSERT, sqlite3_errmsg(db));

    // Clean data
    sqlite3_finalize(stmt);
    return result;
}

// bool queryAccount(sqlite3 *db, const char *description, PasswordInfo *info, EncryptedData *enc)
// {
//     // SQL query satement
//     //const char *sql = "SELECT username, salt, nonce";
// }

bool listAccounts(sqlite3 *db, const char *searchQuery, ListCallback callback, void *user_data)
{
    // Sql template
    const char *sql = "SELECT description, username FROM accounts WHERE description LIKE ? ORDER BY description ASC;";
    sqlite3_stmt *stmt;
    char *likePattern = NULL;
    bool success = true;

    // Prepare
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, ERR_PREPARE, sqlite3_errmsg(db));
        return false;
    }

    // Construct LIKE 
    // If searchQuery exist, pattern the keyword
    if (searchQuery && strlen(searchQuery) > 0)
    {
        // Allocate memory for likePattern (% searchQuery % \0)
        size_t len = strlen(searchQuery) + 3;
        likePattern = (char *)malloc(len);
        if (!likePattern)
        {
            sqlite3_finalize(stmt);
            return false;
        }
        snprintf(likePattern, len, "%%%s%%", searchQuery);
    }
    // No keyword, pattern all
    else
    {
        likePattern = (char *)malloc(2);
        if (!likePattern)
        {
            sqlite3_finalize(stmt);
            return false;
        }
        strcpy(likePattern, "%");
    }

    // Bind argument
    sqlite3_bind_text(stmt, 1, likePattern, -1, SQLITE_TRANSIENT);

    // Loop for querying
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        // Get data found
        const unsigned char *description = sqlite3_column_text(stmt, 0);
        const unsigned char *username = sqlite3_column_text(stmt, 1);

        // Call callback function
        if (callback)
            callback((const char *)description, (const char *)username, user_data);
    }

    // Check error
    if (sqlite3_errcode(db) != SQLITE_DONE)
    {
        fprintf(stderr, ERR_LIST, sqlite3_errmsg(db));
        success = false;
    }

    // Clear
    free(likePattern);
    sqlite3_finalize(stmt);
    
    return success;
}

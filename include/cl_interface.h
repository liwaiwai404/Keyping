/**
 * @file cl_interface.h
 * @brief Prompt user for password information or display information by mode.
 */

#ifndef CL_INTERFACE_H
#define CL_INTERFACE_H

#include <stdbool.h>
#include "cl_parser.h"

/**
 * @brief Limit length of information.
 */
#define MAX_PASSWORDINFO_LEN 256

/**
 * @brief Error information
 */
#define ERR_GETINFO "[Usage error] INFO: Get password information failed.\n"
#define ERR_ENTER_AGAIN "[Usage error] INFO: The two passwords did not match.\n"

/**
 * @brief Success information
 */
#define SUCCESS_ADD "[Success] An account has been added.\n"
#define SUCCESS_LIST "[Success] Accounts has been listed.\n"
#define SUCCESS_QUERY "[Success] Password has been found.\n"
#define SUCCESS_MODIFY "[Success] Password for this account has been modified.\n"
#define SUCCESS_DELETE "[Success] Account has been deleted.\n"


/**
 * @brief Password information
 */
typedef struct
{
    // Account description
    char description[MAX_PASSWORDINFO_LEN];
    
    // Username
    char username[MAX_PASSWORDINFO_LEN];

    // password
    char password[MAX_PASSWORDINFO_LEN];

    // master key
    char masterKey[MAX_PASSWORDINFO_LEN];
} PasswordInfo;


/**
 * @brief Prompt user for information by mode.
 * 
 * @param passwordInfo Point to information.
 * @return true, if get information successfully.
 * @return false, if get information failed.
 */
bool getPasswordInfo(PasswordInfo *passwordInfo, enum Mode mode);

/**
 * @brief Print a row of data include desc, username, or other additional info
 * 
 * @param description Description found
 * @param username Username found
 * @param user_data Point to other additional data
 */
void printDataByRow(const char *description, const char *username, void *user_data);

/**
 * @brief Print data table header
 */
void printHeader(void);

/**
 * @brief Print data table footer
 */
void printFooter(void);

/**
 * @brief Print success information by mode
 * 
 * @param mode Operation mode
 */
void printSuccessInfo(enum Mode mode);

#endif // CL_INTERFACE_H
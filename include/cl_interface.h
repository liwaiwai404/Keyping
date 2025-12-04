/**
 * @file cl_interface.h
 * @brief Prompt user for password information or display information by mode.
 */

#ifndef CL_INTERFACE_H
#define CL_INTERFACE_H

#include <stdbool.h>
#include <stdio.h>
#include "cl_parser.h"
#include "utils.h"

/**
 * @brief Limit length of information.
 */
#define MAX_PASSWORDINFO_LEN 256

/**
 * @brief Error information
 */
#define ERR_GETINFO "[Usage error] INFO: Get password information failed.\n"


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


#endif // CL_INTERFACE_H
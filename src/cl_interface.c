#include "cl_interface.h"

bool getPasswordInfo(PasswordInfo *passwordInfo, enum Mode mode)
{
    // List operator don't need information
    if (mode == LIST)
    {
        fprintf(stderr, ERR_GETINFO);
        return false;
    }

    // Prompt user for description
    printf("[Description]>>>");
    if (!getInfo(passwordInfo->description, sizeof(passwordInfo->description)))
    {
        fprintf(stderr, ERR_GETINFO);
        return false;
    }
    
    // Prompt user for username
    printf("[Username]>>>");
    if (!getInfo(passwordInfo->username, sizeof(passwordInfo->username)))
    {
        fprintf(stderr, ERR_GETINFO);
        return false;
    }
    
    switch (mode)
    {
    case ADD:
        // Prompt user for password
        printf("[Password]>>>");
        if (!getInfo(passwordInfo->password, sizeof(passwordInfo->password)))
        {
            fprintf(stderr, ERR_GETINFO);
            return false;
        }
        return true;
    case QUERY:
    case MODIFY:
    case DELETE:
        // Prompt user for master key
        printf("[Key]>>>");
        if (!getInfo(passwordInfo->masterKey, sizeof(passwordInfo->masterKey)))
        {
            fprintf(stderr, ERR_GETINFO);
            return false;
        }
        return true;
    default:
        fprintf(stderr, ERR_GETINFO);
        return false;
    }

    return true;
}




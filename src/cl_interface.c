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
    

    // Add mode need password infomation
    if (mode == ADD || mode == MODIFY)
    {
        // Prompt user for password
        if (mode == ADD)
            printf("[Password]>>>");
        else
            printf("[New Password]>>>");
        
        if (!getInfo(passwordInfo->password, sizeof(passwordInfo->password)))
        {
            fprintf(stderr, ERR_GETINFO);
            return false;
        }
    }

    // Delete mode not need key
    if (mode != DELETE)
    {
        // Prompt user for master key
        printf("[Key]>>>");
        if (!getInfo(passwordInfo->masterKey, sizeof(passwordInfo->masterKey)))
        {
            fprintf(stderr, ERR_GETINFO);
            return false;
        }
    }
    
    return true;
}

void printDataByRow(const char *description, const char *username, void *user_data)
{
    user_data = NULL;
    if (user_data){}
    printf("| %20s | %20s |\n", description, username);
    printf("+----------------------+----------------------+\n");
}




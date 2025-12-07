#include "cl_interface.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"


bool getPasswordInfo(PasswordInfo *passwordInfo, enum Mode mode)
{
    // List operator don't need information
    if (mode == LIST)
    {
        fprintf(stderr, ERR_GETINFO);
        return false;
    }

    // Prompt user for description
    printf(ANSI_COLOR_CYAN ANSI_STYLE_BOLD"[Description]>>>"ANSI_COLOR_RESET);
    if (!getInfo(passwordInfo->description, sizeof(passwordInfo->description)))
    {
        fprintf(stderr, ERR_GETINFO);
        return false;
    }
    
    // Prompt user for username
    printf(ANSI_COLOR_CYAN ANSI_STYLE_BOLD"[Username]>>>"ANSI_COLOR_RESET);
    if (!getInfo(passwordInfo->username, sizeof(passwordInfo->username)))
    {
        fprintf(stderr, ERR_GETINFO);
        return false;
    }
    

    // Add mode need password infomation
    if (mode == ADD || mode == MODIFY)
    {
        // Prompt user for password
        char tempPassword1[MAX_PASSWORDINFO_LEN];
        if (mode == ADD)
            printf(ANSI_COLOR_MAGENTA "[Password]>>>" ANSI_COLOR_RESET);
        else
            printf(ANSI_COLOR_MAGENTA "[New Password]>>>" ANSI_COLOR_RESET);
        
        if (!getInfoSecure(tempPassword1, sizeof(tempPassword1)))
        {
            fprintf(stderr, ERR_GETINFO);
            return false;
        }

        // Prompt user for password again
        char tempPassword2[MAX_PASSWORDINFO_LEN];
        printf(ANSI_COLOR_MAGENTA "[Enter again]>>>" ANSI_COLOR_RESET);

        if (!getInfoSecure(tempPassword2, sizeof(tempPassword2)))
        {
            fprintf(stderr, ERR_GETINFO);
            return false;
        }

        if (strcmp(tempPassword1, tempPassword2) != 0)
        {
            fprintf(stderr, ERR_ENTER_AGAIN);
            return false;
        }

        // Store info
        memcpy(passwordInfo->password, tempPassword1, sizeof(passwordInfo->password));
    }

    // Delete mode not need key
    if (mode != DELETE)
    {
        // Prompt user for master key
        printf(ANSI_COLOR_YELLOW ANSI_STYLE_BOLD"[Key]>>>"ANSI_COLOR_RESET);
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

void printHeader(void)
{
    printf("+---------------------------------------------+\n");
    printf("|                Account list                 |\n");
    printf("+----------------------+----------------------+\n");
    printf("|          Description |             Username |\n");
    printf("+----------------------+----------------------+\n");
}

void printFooter(void)
{
    printf("+----------------------+----------------------+\n");
}

void printSuccessInfo(enum Mode mode)
{
    switch (mode)
    {
    case ADD:
        printf(SUCCESS_ADD);
        break;
    case LIST:
        printf(SUCCESS_LIST);
        break;
    case QUERY:
        printf(SUCCESS_QUERY);
        break;
    case MODIFY:
        printf(SUCCESS_MODIFY);
        break; 
    case DELETE:
        printf(SUCCESS_DELETE);
        break;   
    default:
        break;
    }
}




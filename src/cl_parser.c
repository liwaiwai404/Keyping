#include "cl_parser.h"
#include <stdio.h>
#include <string.h>


bool checkFormat(const int argc, char **argv)
{
    // Basic argument count validation
    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, ERR_ARGNUM);
        return false;
    }

    char *flag = argv[1];

    // Flag must start with '-' and be exactly 2 characters long (e.g., "-a")
    if (flag[0] != '-' || strlen(flag) != 2)
    {
        fprintf(stderr, ERR_ARGFORM);
        return false;
    }

    // Special restriction: Only 'LIST' mode (-l) supports an optional 3rd argument
    // e.g. "./keyping -l google" is valid, but "./keyping -a google" is not currently supported here.
    if (argc == 3 && flag[1] != 'l')
    {
        fprintf(stderr, ERR_ARGMODE);
        return false;
    }

    // Mode validation
    switch (flag[1])
    {
        case 'l':
        case 'a':
        case 'q':
        case 'm':
        case 'd':
            return true;
        default:
            fprintf(stderr, ERR_ARGMODE);
            return false;
    }  
}

enum Mode getMode(char mode)
{
    switch (mode)
    {
        case 'l':
            return LIST;
        case 'a':
            return ADD;
        case 'q':
            return QUERY;
        case 'm':
            return MODIFY;
        case 'd':
            return DELETE;
        default:
            // Should be caught by checkFormat, but safe fallback
            fprintf(stderr, ERR_ARGMODE);
            return 0;
    }
}


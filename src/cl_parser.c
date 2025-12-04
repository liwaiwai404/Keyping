#include "cl_parser.h"

/*
* Check command line arguments format
*/
bool checkFormat(const int argc, char **argv)
{
    // Check the arguments count
    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, ERR_ARGNUM);
        return false;
    }

    // Get mode flag 
    char *flag = argv[1];

    // Check flag form(Flag is begin with '-' and length is not 2)
    if (flag[0] != '-' || strlen(flag) != 2)
    {
        fprintf(stderr, ERR_ARGFORM);
        return false;
    }

    // Special situation
    if (argc == 3 && flag[1] != 'l')
    {
        fprintf(stderr, ERR_ARGMODE);
        return false;
    }

    // Check the mode
    switch (flag[1])
    {
        // Right mode charactor
        // l mode can be single or carry a description
        case 'l':
        // other mode must be single
        case 'a':
        case 'q':
        case 'm':
        case 'd':
            return true;
        // Other charactor is wrong
        default:
            fprintf(stderr, ERR_ARGMODE);
            return false;
    }  
}



/*
* Get the accurate mode
*/
enum Mode getMode(char mode)
{
    // Return Mode value by accurate mode
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
            fprintf(stderr, ERR_ARGMODE);
            return 0;
    }
}


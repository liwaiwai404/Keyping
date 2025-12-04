#ifndef CL_PARSER_H
#define CL_PARSER_H

// Header file
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Macro definition
// Error infomation
#define ERR_ARGNUM "[Usage error] INFO: wrong number of arguments\n"
#define ERR_ARGFORM "[Usage error] INFO: wrong form of arguments\n"
#define ERR_ARGMODE "[Usqge error] INFO: wrong mode\n"

// Use mode by CL arguments
enum Mode
{
    ADD = 1,
    LIST,
    QUERY,
    MODIFY,
    DELETE
};

// Return true if command line arguments is right format
// else return false 
bool checkFormat(const int argc, char **argv);

// Return mode value if parameter is a right mode
// else return 0
enum Mode getMode(char mode);

#endif
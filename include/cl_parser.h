/**
 * @file cl_parser.h
 * @brief Command Line Interface (CLI) parser module.
 * @details Handles the parsing and validation of command-line arguments for the Keyping application.
 */

#ifndef CL_PARSER_H
#define CL_PARSER_H
 
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Error messages for invalid command-line usage.
 */
#define ERR_ARGNUM "[Usage error] INFO: wrong number of arguments\n"
#define ERR_ARGFORM "[Usage error] INFO: wrong form of arguments\n"
#define ERR_ARGMODE "[Usage error] INFO: wrong mode\n"

/**
 * @brief Application operation modes derived from CLI arguments.
 */
enum Mode
{
    ADD = 1,
    LIST,
    QUERY,
    MODIFY,
    DELETE
};

/**
 * @brief Validates the format of command-line arguments.
 *
 * Checks if the number of arguments and the structure of the mode flag
 * (e.g., length, prefix) are correct.
 *
 * @param argc Argument count from main().
 * @param argv Argument vector from main().
 * @return true if the arguments are valid.
 * @return false if the argument count is wrong or the format is invalid.
 */ 
bool checkFormat(const int argc, char **argv);

/**
 * @brief Parses the mode character into a Mode enum.
 *
 * @param mode The character representing the mode (e.g., 'a', 'l').
 * @return The corresponding Mode enum value (e.g., ADD, LIST).
 * @return 0 if the mode character is unrecognized.
 */
enum Mode getMode(char mode);

#endif // CL_PARSER_H
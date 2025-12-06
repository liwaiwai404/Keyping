/**
 * @file utils.h
 * @brief Provid util function for other functions
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>


/**
 * @brief Read a line of data safely
 * 
 * @param buffer Point to the data to be read
 * @param size Limit size of data read
 */
bool readLine(char *buffer, int size);

/**
 * @brief Get password information reaptely until right input
 * 
 * @param info Point to information
 * @param size Limit size of information
 */
bool getInfo(char *info, int size);

#endif // UTILS_H
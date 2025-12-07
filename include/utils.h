/**
 * @file utils.h
 * @brief Provid util function for other functions
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

/**
 * @brief Error information
 */
#define ERR_COPY "[Copy error] INFO: Copy failed.\n"
#define ERR_MISS_CLIP "[Copy error] No clipboard tool found.\n"

/**
 * @brief Success information
 */
#define SUCCESS_COPY "[Success] Password has been copied to clipboard.\n"

// ANSI Color define
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// ANSI Style define
#define ANSI_STYLE_BOLD    "\x1b[1m"

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

/**
 * @brief Securely read a password without echoing characters.
 * 
 * @param buffer Buffer to store the password.
 * @param size Size of the buffer.
 * @return true if read successfully.
 */
bool getInfoSecure(char *buffer, int size);

/**
 * @brief Copy text to Windows clipboard (WSL specific)
 * 
 * @param plaintext The text to copy
 */
void copyToClipboard(const char *plaintext);

/**
 * @brief Try to execute clipboard tool to store plaintext
 * 
 * @param cmd Clipboard tool
 * @param plaintext Plaintext of password
 */
bool tryCommand(const char *cmd, const char *plaintext);

#endif // UTILS_H
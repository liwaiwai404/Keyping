#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

bool readLine(char *buffer, int size)
{
    // Read a line of data
    if (fgets(buffer, size, stdin) == NULL)
    {
        // If read failed or EOF 
        buffer[0] = '\0';
        return false;
    }

    // Remove trailing '\n' or space char
    size_t len = strlen(buffer);
    while (len > 0 && isspace((unsigned char)buffer[len - 1]))
    {
        buffer[len - 1] = '\0';
        --len;
    }

    // Remove heading space char
    // start will point to the first no-space char in the end
    char *start = buffer;
    while (*start && isspace((unsigned char)*start))
    {
        ++start;
    }

    // Check if is empty
    if (*start == '\0')
    {
        buffer[0] = '\0';
        return false;
    }

    // Move data pointed by start to buffer
    if (start != buffer)
        memmove(buffer, start, strlen(start) + 1);
    
    return true;
}

bool getInfo(char *info, int size)
{
    while (true)
    {
        // Read a line of data
        if (readLine(info, size))
            return true;
        
        // If read failed because of EOF
        if (feof(stdin))
        {
            clearerr(stdin);
            return false;
        }

        // Not EOF
        printf("[Wrong]>>>");
    }
}
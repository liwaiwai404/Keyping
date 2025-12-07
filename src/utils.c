#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <termios.h> 
#include <unistd.h>  
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

bool getInfoSecure(char *buffer, int size)
{
    struct termios oldt, newt;
    bool success = false;

    // Get current terminal attributes
    if (tcgetattr(STDIN_FILENO, &oldt) != 0)
    {
        return false;
    }

    // Copy the attribute and modify it: Turn off echo (ECHO)
    newt = oldt;
    newt.c_lflag &= ~ECHO;

    // Set new attributes
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &newt) != 0)
    {
        return false;
    }

    // Read password
    if (fgets(buffer, size, stdin) != NULL)
    {
        size_t len = strlen(buffer);
        // Remove '\n'
        if (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
        }
        success = true;
    }

    // Restore the terminal to its original properties (enable echo).
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    // Newline
    printf("\n");

    return success;
}

void copyToClipboard(const char *plaintext)
{
    // Call clip.exe program
    FILE *pipe = popen("clip.exe", "w");
    
    // Access clip
    if (pipe != NULL)
    {
        // Pass plaintext to clip.exe 
        fprintf(pipe, "%s", plaintext);
        
        // Close pipe
        if (pclose(pipe) == 0)
        {
            printf(SUCCESS_COPY);
        }
        else
        {
            fprintf(stderr, ERR_COPY);
        }
    }
    else
    {
        fprintf(stderr, ERR_COPY);
    }
}
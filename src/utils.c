#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
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
    bool success = false;
    
    // Windows
    #if defined(_WIN32) || defined(_WIN64)
        success = tryCommand("clip", plaintext);
    // MacOS
    #elif defined(__APPLE__)
        success = tryCommand("pbcopy", plaintext);
    // Linux (include WSL)
    #elif defined(__linux__)
        // Try Wayland 
        if (!success)
            success = tryCommand("wl-copy 2>/dev/null", plaintext);
        
        // Try X11
        if (!success)
            success = tryCommand("xclip -selection clipboard 2>/dev/null", plaintext);
        if (!success)
            success = tryCommand("xsel --clipboard --input 2>/dev/null", plaintext);

        // WSL (WSL can call clip.exe)
        if (!success)
            success = tryCommand("clip.exe 2>/dev/null", plaintext);
    #else
        fprint(stderr, ERR_COPY);
        return;
    #endif

    if (success)
        printf(SUCCESS_COPY);
    else
    {
        // Prompt users on Linux to install missing tools
        #ifdef __linux__
            fprintf(stderr, ERR_MISS_CLIP);
        #else
            fprintf(stderr, ERR_COPY); // 使用你 utils.h 中定义的宏
        #endif
    }
}

bool tryCommand(const char *cmd, const char *plaintext)
{
    // Connect to command line clipboard tool
    FILE *pipe = popen(cmd, "w");

    // Access clipboard tool
    if (pipe == NULL)
        return false;

    // Pass plaintext to clipboard tool
    fprintf(pipe, "%s", plaintext);
        
    // Close pipe
    if (pclose(pipe) != 0)
        return false;

    return true;
}

// Makesure the path exist
static void ensureDirectoryExists(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        // If the dir not exist, try to create
        // Permission 0700: Read and write only for the current user
        mkdir(path, 0700);
    }
}

void getAppDBPath(char *buffer, size_t size) {
    const char *homeDir = getenv("HOME");
    
    if (homeDir == NULL) {
        // Can't find home dir, back to current path
        snprintf(buffer, size, "keyping.db");
        return;
    }

    // Construct base path ~/.local/share
    char basePath[512];
    snprintf(basePath, sizeof(basePath), "%s/.local", homeDir);
    ensureDirectoryExists(basePath);

    snprintf(basePath, sizeof(basePath), "%s/.local/share", homeDir);
    ensureDirectoryExists(basePath);

    // Construct application path ~/.local/share/keyping
    char appPath[512];
    snprintf(appPath, sizeof(appPath), "%s/.local/share/keyping", homeDir);
    ensureDirectoryExists(appPath);

    // Get the final database path
    snprintf(buffer, size, "%s/keyping.db", appPath);
}
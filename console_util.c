
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include "console_util.h"


// Reads a line of input from stdin and removes the trailing newline
// Caller is responsible for allocating and deallocating the buffer
int get_line_static(char* buffer, int buf_size)
{
    // Read user input into the buffer and trim the trailing newline
    if (fgets(buffer, buf_size, stdin) == NULL)
    {
        debug("Cannot fgets() from stdin");
        return EXIT_FAILURE;
    }
    // Find the index of the first newline character in the string
    int index = strcspn(buffer, "\n");
    buffer[index] = '\0';

    return EXIT_SUCCESS;
}


// Reads a line of input from stdin and removes the trailing newline
// Caller is responsible for deallocating the buffer using the free function
// Uses a the new getline() function that was published in POSIX.1-2008
// Most searches for getline() return references for C++ (incorrect)
int get_line_dynamic(char** buffer, size_t* size)
{
    size_t length = getline(buffer, size, stdin);
    if (length == (size_t)-1)
    {
        debug("cannot getline() from stdin");
        *buffer = NULL;
        *size = 0;
        return EXIT_FAILURE;
    }
    // Find the index of the first newline character in the string
    int index = strcspn(*buffer, "\n");
    (*buffer)[index] = '\0';

    return EXIT_SUCCESS;
}


// Converts a number string like "123" to an integer
int to_integer(char* buffer, int* result)
{
    // Convert the input buffer to a number
    char* nextPtr = NULL;
    long number = strtol(buffer, &nextPtr, 10);
    if (buffer[0] == '\0' || // input was empty
        nextPtr == buffer || // invalid number
        nextPtr[0] != '\0')  // contained more than a number
    {
        debug("strtol() input was not a number");
        return EXIT_FAILURE;
    }
    else if (number > INT_MAX || number < INT_MIN)
    {
        debug("strtol() input was too large to store as an integer");
        return EXIT_FAILURE;
    }

    *result = (int)number;
    return EXIT_SUCCESS;
}


// Converts a number string like "123.45" to an integer
int to_float(char* buffer, float* result)
{
    // Convert the input buffer to a number
    char* nextPtr = NULL;
    double number = strtof(buffer, &nextPtr);
    if (buffer[0] == '\0' || // input was empty
        nextPtr == buffer || // invalid number
        nextPtr[0] != '\0')  // contained more than a number
    {
        debug("strtof() input was not a number");
        return EXIT_FAILURE;
    }
    else if (number > FLT_MAX || number < FLT_MIN)
    {
        debug("strtof() input was too large to store as an integer");
        return EXIT_FAILURE;
    }

    *result = (float)number;
    return EXIT_SUCCESS;
}

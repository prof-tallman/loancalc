
#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>

// macro to print the filename and line number for each error message
#define debug(msg) fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, msg);

// Reads a line of input from stdin and removes the trailing newline
// Caller is responsible for allocating and deallocating the buffer
int get_line_static(char* buffer, int buf_size);

// Reads a line of input from stdin and removes the trailing newline
// Caller is responsible for deallocating the buffer using the free function
int get_line_dynamic(char** buffer, size_t* size);

// Converts a number string like "123" to an integer
int to_integer(char* buffer, int* result);

// Converts a number string like "123.45" to an integer
int to_float(char* buffer, float* result);

#endif

// <Insert project name here>
// Error (header)
// (c) 2018 Jani Nykänen

#ifndef __ERROR__
#define __ERROR__

// Error buffer size
#define ERR_BUFFER_SIZE 1024

#include "stdbool.h"

// Initialize error system
void error_init();

// Throw an error
void error_throw(const char* msg, const char* param);

// Get error
char* error_get_message();

// Has error
bool has_error();

// Remove error
void error_flush();

// Set callback for showing errors
void error_set_callback(void (*cb) (const char*));

// Show the error message
void error_show();

// "Memory allocation error!"
void error_mem_alloc();

#endif // __ERROR__

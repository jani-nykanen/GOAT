// GOAT
// Error (source)
// (c) 2018 Jani Nykänen

#include "error.h"

#include "stdio.h"
#include "stdlib.h"

// Error message
static char errMessage [ERR_BUFFER_SIZE];
// Is there an error
static bool hasError;
// Error showing callback
static void (*fnErr) (const char*);


// Initialize
void error_init() {

    hasError = 0;
}


// Throw an error
void error_throw(const char* msg, const char* param) {

    // Store error message
    if(param == NULL) {

        snprintf(errMessage,ERR_BUFFER_SIZE,"%s",msg);
    }
    else {

        snprintf(errMessage,ERR_BUFFER_SIZE,"%s%s",msg, param);
    }
    
    hasError = true;
}


// Get message
char* error_get_message() {

    hasError = 0;
    return errMessage;
}


// Has error
bool has_error() {

    return hasError;
}


// Get rid of the error
void error_flush() {

    hasError = 0;
}


// Set callback for showing errors
void error_set_callback(void (*cb) (const char*)) {

    fnErr = cb;
}

// Show the error message
void error_show() {

    if(!hasError || fnErr == NULL) 
        return;

    fnErr(error_get_message() );
}


// "Memory allocation error!"
void error_mem_alloc() {

    error_throw("Memory allocation error!", NULL);
}
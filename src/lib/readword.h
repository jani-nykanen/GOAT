// A small library for getting words in a text file
// (Header)
// (c) 2018 Jani Nykänen

#ifndef LIB__READWORD
#define LIB__READWORD

#include <stdio.h>
#include <stdbool.h>

// Initial word length
#define INITIAL_WORD_LENGTH 64

// Error buffer size
#define ERROR_BUFFER_SIZE 256

// Word reader type
typedef struct {

    FILE* fhandle;
    bool opened;
    char* buffer;
    size_t bufferSize;
    char commentChar;
    char error[ERROR_BUFFER_SIZE];
    size_t charPointer;
}
WORDREADER;

// Set word reader
WORDREADER* wr_create(char comment);

// Open file for reading
int wr_open(WORDREADER* wr, const char* path);

// Read the next word
int wr_read_next(WORDREADER* wr);

// Close file
void wr_close(WORDREADER* wr);

// Destroy a word reader
void wr_destroy_reader(WORDREADER* wr);

#endif // LIB__READWORD

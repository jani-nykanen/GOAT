// A small library for getting words in a text file
// (Source)
// (c) 2018 Jani Nykänen

#include "readword.h"

#include <stdlib.h>


// Reallocate buffer
static int reallocate(WORDREADER* wr) {

    // Create a new buffer
    size_t nsize = INITIAL_WORD_LENGTH + wr->bufferSize;
    char* newBuffer = (char*)malloc(nsize);
    if(newBuffer == NULL) {

        return 1;
    }

    // Copy
    int i = 0;
    for(; i < wr->bufferSize; ++ i) {

        newBuffer[i] = wr->buffer[i];
    }

    // Delete old buffer & set the new buffer
    free(wr->buffer);
    wr->buffer = newBuffer;
    wr->bufferSize = nsize;

    return 0;
}


// Add a character to the buffer
static int add_char(WORDREADER* wr, char c) {

    // If not enough size in the buffer, reallocate
    wr->buffer[wr->charPointer ++] = c;
    while(wr->charPointer >= wr->bufferSize-1) {

        if(reallocate(wr) == 1) {

            return 1;
        }
    }

    return 0;
}


// Set word reader
WORDREADER* wr_create(char comment) {

    // Allocate memory
    WORDREADER* wr = (WORDREADER*)malloc(sizeof(WORDREADER));
    if(wr == NULL) {

        return NULL;
    }
    wr->buffer = (char*)malloc(INITIAL_WORD_LENGTH);
    if(wr->buffer == NULL) {

        return NULL;
    }
    wr->bufferSize = INITIAL_WORD_LENGTH;

    wr->opened = false;
    wr->fhandle = NULL;
    wr->commentChar = comment;

    return wr;
}


// Open file for reading
int wr_open(WORDREADER* wr, const char* path) {

    if(wr == NULL) return 1;
    if(wr->opened) return 0;

    // Open file
    wr->fhandle = fopen(path, "r");
    if(wr->fhandle == NULL) {

        snprintf(wr->error,ERROR_BUFFER_SIZE, "Failed to load a file in: %s!", path);
        return 1;
    }
    wr->opened = true;
    wr->charPointer = 0;

    return 0;
}


// Read the next word
int wr_read_next(WORDREADER* wr) {

    if(wr == NULL || !wr->opened) return 0;

    // Clear buffers
    wr->charPointer = 0;
    wr->buffer[wr->charPointer] = '\0';

    bool isComment =false;
    bool isQuote =false;
    char quoteChar;

    // Get characters
    char c;
    while ((c = fgetc(wr->fhandle)) != EOF) {

        // If comment, break
        if(c == wr->commentChar) {

            isComment = true;
            break;
        }

        // Handle quotes
        if(!isQuote && (c == 39 || c == '"')) {
        
            isQuote = true;
            quoteChar = c;
            continue;
        }
        else if(isQuote) {

            if(c == quoteChar) {

                break;
            }
            else {

                if(add_char(wr, c) == 1)
                    return 0;

                continue;
            }
        }

        // If not "weird" character, push to the buffer
        if(c != ' ' && c != '\t' && c != ',' && c != '\n') {

            if(add_char(wr, c) == 1)
                return 0;
        }
        // If at leats one character, we have a word
        else if(wr->charPointer > 0) {

            break;
        }
    }

    // If comment, skip until a new line
    if(isComment) {

        while(1) {

            c = fgetc(wr->fhandle);
            if(c == EOF || c == '\n')
                break;
        }
    }

    // If not the end of the file and no characters, try again
    if(c != EOF && wr->charPointer == 0) {

        return wr_read_next(wr);
    }

    // Null terminate
    wr->buffer[wr->charPointer ++] = '\0';
    
    // Return 1, if end of the file, so user knows to stop reading
    if(c == EOF) {

        // Also, close the file
        wr_close(wr);

        return 0;
    }

    return 1;
}


// Close file
void wr_close(WORDREADER* wr) {

    if(wr == NULL || !wr->opened) return;

    wr->opened = false;
    fclose(wr->fhandle);
}


// Destroy a word reader
void wr_destroy_reader(WORDREADER* wr) {

    if(wr == NULL) return;

    free(wr->buffer);
    free(wr);
}

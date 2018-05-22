// GOAT
// Leaderboards (source)
// (c) 2018 Jani Nykänen

#include "leaderboard.h"

#include <curl/curl.h>

#include "../include/std.h"
#include "../include/system.h"

// Return buffer size
#define RET_BUFFER_SIZE 1024
// Word length
#define WORD_LENGTH 32
// Check value modulo
#define CHECK_MOD 1023
// Check value multiplier
#define CHECK_MUL 255

// Word type
typedef char _WORD[WORD_LENGTH];

// Handle
static CURL* handle;
// Address
static char* address;
// Address length
static size_t addrLength;

// Return buffer
static char buffer[RET_BUFFER_SIZE];
// Buffer pointer
static size_t bufptr;


// Clear buffer
static void clear_buffer() {

    memset(buffer,'\0', RET_BUFFER_SIZE);
}


// Get the next word from the buffer, starting from certain index
size_t next_word(int index, _WORD w) {

    // Clear word
    memset(w, '\0', WORD_LENGTH);

    // Copy characters
    int i = (int)index;
    int windex = 0;
    for(; i < RET_BUFFER_SIZE && buffer[i] != '\0' && buffer[i] != '\n'; ++ i) {

        w[windex ++] = buffer[i];
    }

    if(buffer[i] == '\0' || i == RET_BUFFER_SIZE-1)
        return -1;

    return i+1;
}


// Receive
static size_t receive(void* data, size_t size, size_t mem, void* user) {

    // Data in characters
    char* chr = (char*)data;
    size_t len = size * mem;

    // Copy received data to the buffer
    int i = 0;
    for(; i < len; ++ i) {

        // TODO: Better error handling
        if(bufptr >= RET_BUFFER_SIZE) {

            error_throw("Buffer overflow", NULL);
            return 0;
        }

        buffer[bufptr ++] = chr[i];
    }

    return size * mem;
}


// Send a request
static int send_request(const char* req) {

    // Set URL
    char fullAddress[1024];
    snprintf(fullAddress, 1024, "%s/?%s", address, req);
    curl_easy_setopt(handle, CURLOPT_URL, fullAddress);

    // Clear buffer
    bufptr = 0;
    clear_buffer();

    // Send request & handle data
    CURLcode success = curl_easy_perform(handle);
    if(success != CURLE_OK) {

        char errCode[16];
        snprintf(errCode, 16, "%d", (int)success);
        error_throw("Failed to send an\nhttp request!\nError code:", errCode);
        return 1;
    }

    return 0;
}


// Get leaderboards
static void get_lb(LEADERBOARD* lb, int index) {

    int i = 0;
    int lbi = 0;
    _WORD w;

    // Get names & scores
    while(index != -1) {

        index = next_word(index, w);

        if(strcmp(w, "") == 0)
            continue;

        // Store data
        if(i % 2 == 0) {

            strcpy(lb->names[lbi], w);
        }
        else {

            lb->scores[lbi] = (int)strtol(w, NULL, 10);
            ++ lbi;
        }

        ++ i;
    }
}


// Initialize http request system
int lb_init_http(const char* addr) {

    // Create handle
    handle = curl_easy_init();
    if(handle == NULL) {

        error_throw("Could not create a handle!", NULL);
        return 1;
    }

    // Set receiver
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, receive);

    // Copy address
    addrLength = strlen(addr);
    address = (char*)malloc(sizeof(char) * addrLength);
    if(address == NULL) {

        error_mem_alloc();
        return 1;
    }
    strcpy(address, addr);

    return 0;
}


// Get leaderboard
int lb_get(LEADERBOARD* lb) {

    // Send request
    if(send_request("&mode=get") == 1) {

        return 1;
    }

    // Get words
    _WORD w;
    int index = 0;

    // Get first word
    index = next_word(index, w);
    if(strcmp(w,"true") != 0) {

        error_throw("Expected true, got ", w);
        return 1;
    }

    // Get leaderboards
    get_lb(lb, index);

    return 0;
}


// Add score
int lb_add_score(LEADERBOARD* lb, const char* name, int score) {
    
    // Send request
    char str[1024];
    snprintf(str, 1024, "&mode=set&name=%s&score=%d&check=%d", name, score, 
        (score * CHECK_MUL) % CHECK_MOD);
    if(send_request(str) == 1) {

        return 1;
    }

    // Get first word
    _WORD w;
    int index = 0;
    index = next_word(index, w);
    if(strcmp(w,"true") != 0) {

        error_throw("Expected true, got ", w);
        return 1;
    }

    // Get leaderboards
    get_lb(lb, index);

    return 0;
}

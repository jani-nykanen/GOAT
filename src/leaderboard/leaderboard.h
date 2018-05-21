// GOAT
// Leaderboards (header)
// (c) 2018 Jani Nykänen

#ifndef __LEADERBOARDS__
#define __LEADERBOARDS__

// Leaderboard name length
#define _NAME_LENGTH 32

// Max amount of members
#define MEMBER_MAX 10

typedef char LB_NAME[_NAME_LENGTH];

// Leaderboard type
typedef struct {

    LB_NAME names[MEMBER_MAX];
    int scores[MEMBER_MAX];
}
LEADERBOARD;

// Initialize http request system
int lb_init_http(const char* address);

// Get leaderboard
int lb_get(LEADERBOARD* lb);

// Add score
int lb_add_score(LEADERBOARD* lb, const char* name, int score);

#endif // __LEADERBOARDS__

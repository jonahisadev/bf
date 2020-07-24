#pragma once

#ifndef BF_SIZE
    #define BF_SIZE 1024 * 1024
#endif

struct _BFState
{
    char* source;
    int dp;
    char data[BF_SIZE];
};
typedef struct _BFState BFState;

BFState* bf_load(const char* path);
void bf_run(BFState* bf);
void bf_free(BFState* bf);
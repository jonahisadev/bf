#include "bf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BFState* bf_load(const char* path)
{
    BFState* bf = (BFState*) malloc(sizeof(BFState));
    memset(bf->data, 0, BF_SIZE);
    bf->dp = 0;

    FILE* file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Could not open file '%s'\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    bf->source = (char*) malloc(sizeof(char) * size + 1);
    fread(bf->source, 1, size, file);
    fclose(file);

    bf->source[size] = '\0';
    return bf;
}

void bf_run(BFState* bf)
{
    int ip = 0;
    int s_len = strlen(bf->source);

    while (ip <= s_len)
    {
        char c = bf->source[ip];

        switch (c)
        {
            case '>':
                bf->dp++; break;

            case '<':
                bf->dp--; break;

            case '+':
                bf->data[bf->dp]++; break;
            
            case '-':
                bf->data[bf->dp]--; break;

            case '.':
                putc(bf->data[bf->dp], stdout); break;
            
            case ',':
                bf->data[bf->dp] = getc(stdin); break;

            case '[': {
                if (bf->data[bf->dp] == 0) {
                    int nest = 0;
                    while (1) {
                        ++ip;
                        if (bf->source[ip] == ']') {
                            if (nest == 0)
                                break;
                            else
                                nest--;
                        }

                        if (bf->source[ip] == '[')
                            nest++;
                    }
                }
                break;
            }

            case ']': {
                if (bf->data[bf->dp] != 0) {
                    int nest = 0;
                    while (1) {
                        --ip;
                        if (bf->source[ip] == '[') {
                            if (nest == 0)
                                break;
                            else
                                nest--;
                        }

                        if (bf->source[ip] == ']')
                            nest++;
                    }
                }
                break;
            }

            default:
                ++ip;
                break;
        }

        ++ip;
    }
}

void bf_free(BFState* bf)
{
    free(bf->source);
    free(bf);
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Please provide a file to run\n");
        return -1;
    }

    const char* path = argv[1];
    BFState* bf = bf_load(path);
    if (bf) {
        bf_run(bf);
        bf_free(bf);
    } else
        return -1;

    return 0;
}
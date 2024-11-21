#ifndef LEVEL_H
#define LEVEL_H

#include <stddef.h>
#include <stdint.h>

struct Level
{
    int moves;
    char desc[256];
    int cluster;
};

struct Levels
{
    struct Level *entries;
    size_t count;
    size_t capacity;
};

int level_load_file(struct Levels *f, const char *path, uint64_t skip);

int level_load_incard(struct Levels *f, uint64_t skip);

void level_free(struct Levels *f);

#endif

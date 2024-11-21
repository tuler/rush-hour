#ifndef FILE_H
#define FILE_H

#include <stddef.h>
#include <stdint.h>

struct FileEntry
{
    int moves;
    char desc[256];
    int cluster;
};

struct File
{
    struct FileEntry *entries;
    size_t count;
    size_t capacity;
};

int file_load(struct File *f, const char *path, uint64_t skip);

void file_free(struct File *f);

#endif

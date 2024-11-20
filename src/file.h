#ifndef FILE_H
#define FILE_H

#include <stddef.h>

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

int file_load(struct File *f, const char *path);
void file_free(struct File *f);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

#define INITIAL_CAPACITY 16

int file_init(struct File *f)
{
    f->entries = malloc(INITIAL_CAPACITY * sizeof(struct FileEntry));
    if (!f->entries)
        return -1;
    f->count = 0;
    f->capacity = INITIAL_CAPACITY;
    return 0;
}

void file_free(struct File *f)
{
    free(f->entries);
    f->entries = NULL;
    f->count = f->capacity = 0;
}

int file_add_entry(struct File *f, struct FileEntry *entry)
{
    if (f->count >= f->capacity)
    {
        size_t new_capacity = f->capacity * 2;
        struct FileEntry *new_entries = (struct FileEntry *)realloc(f->entries, new_capacity * sizeof(struct FileEntry));
        if (!new_entries)
            return -1;
        f->entries = new_entries;
        f->capacity = new_capacity;
    }
    f->entries[f->count++] = *entry;
    return 0;
}

int file_load(struct File *f, const char *path, uint64_t skip)
{
    FILE *fp = fopen(path, "r");
    if (!fp)
        return -1;

    char line[1024];
    struct FileEntry entry;
    int result = file_init(f);
    if (result != 0)
    {
        fclose(fp);
        return result;
    }

    while (fgets(line, sizeof(line), fp))
    {
        // Skip comments
        if (line[0] == '#')
            continue;

        // Skip lines
        if (skip > 0)
        {
            skip--;
            continue;
        }

        // Parse line
        if (sscanf(line, "%d %255s %d",
                   &entry.moves,
                   entry.desc,
                   &entry.cluster) == 3)
        {
            if (file_add_entry(f, &entry) != 0)
            {
                file_free(f);
                fclose(fp);
                return -1;
            }
        }
        else
        {
            file_free(f);
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);
    return 0;
}

#include <riv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "level.h"

#define INITIAL_CAPACITY 256

size_t buffer_getline(char *line, size_t line_max, const uint8_t *buffer, size_t buffer_len, size_t *pos)
{
    size_t i = 0;

    // Check if we're at buffer end
    if (*pos >= buffer_len)
    {
        return 0;
    }

    // Copy until newline or buffer end
    while (*pos < buffer_len && i < line_max - 1)
    {
        char c = (char)buffer[*pos];
        (*pos)++;

        // Handle line endings
        if (c == '\n')
        {
            break;
        }
        if (c == '\r')
        {
            if (*pos < buffer_len && buffer[*pos] == '\n')
            {
                (*pos)++;
            }
            break;
        }

        line[i++] = c;
    }

    line[i] = '\0';
    return i;
}

int level_init(struct Levels *f)
{
    f->entries = malloc(INITIAL_CAPACITY * sizeof(struct Level));
    if (!f->entries)
        return -1;
    f->count = 0;
    f->capacity = INITIAL_CAPACITY;
    return 0;
}

void level_free(struct Levels *f)
{
    free(f->entries);
    f->entries = NULL;
    f->count = f->capacity = 0;
}

int level_add_entry(struct Levels *f, struct Level *entry)
{
    if (f->count >= f->capacity)
    {
        size_t new_capacity = f->capacity * 2;
        struct Level *new_entries = (struct Level *)realloc(f->entries, new_capacity * sizeof(struct Level));
        if (!new_entries)
            return -1;
        f->entries = new_entries;
        f->capacity = new_capacity;
    }
    f->entries[f->count++] = *entry;
    return 0;
}

int level_load_incard(struct Levels *f, uint64_t skip)
{
    if (riv->incard_len == 0)
        return -1;

    char line[1024];
    struct Level entry;
    int result = level_init(f);
    if (result != 0)
    {
        return result;
    }

    size_t pos = 0;
    while (buffer_getline(line, sizeof(line), riv->incard, riv->incard_len, &pos))
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
            if (level_add_entry(f, &entry) != 0)
            {
                level_free(f);
                return -1;
            }
        }
        else
        {
            level_free(f);
            return -1;
        }
    }

    return 0;
}

int level_load_file(struct Levels *f, const char *path, uint64_t skip)
{
    FILE *fp = fopen(path, "r");
    if (!fp)
        return -1;

    char line[1024];
    struct Level entry;
    int result = level_init(f);
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
            if (level_add_entry(f, &entry) != 0)
            {
                level_free(f);
                fclose(fp);
                return -1;
            }
        }
        else
        {
            level_free(f);
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);
    return 0;
}

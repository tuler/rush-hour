#pragma once

#include <string>
#include <vector>
extern "C"
{
#include "riv.h"
}

typedef struct
{
    uint64_t moves;
    std::string desc;
    int cluster;
} FileEntry;

class File
{
public:
    explicit File(const std::string &path);

    const FileEntry &operator[](size_t index) const;

    size_t size() const;

private:
    std::vector<FileEntry> entries;
};

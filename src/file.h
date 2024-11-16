#pragma once

#include <string>
#include <vector>

typedef struct
{
    int moves;
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

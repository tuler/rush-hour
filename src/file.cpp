#include "file.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

File::File(const std::string &path)
{
    // open file
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + path);
    }

    std::string line;

    // read file line by line
    while (std::getline(file, line))
    {
        if (line[0] == '#')
        {
            // comment out
            continue;
        }
        std::stringstream ss(line);
        FileEntry entry;

        // each line contains number of moves, board description and cluster count
        if (ss >> entry.moves >> entry.desc >> entry.cluster)
        {
            entries.push_back(entry);
        }
        else
        {
            throw std::runtime_error("Invalid line format in file: " + path);
        }
    }

    file.close();
}

const FileEntry &File::operator[](size_t index) const
{
    return entries[index];
}

size_t File::size() const
{
    return entries.size();
}

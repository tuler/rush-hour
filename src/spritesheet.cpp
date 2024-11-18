#include "spritesheet.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

SpriteSheet::SpriteSheet(std::string imagePath, std::string dataPath, int64_t alpha)
{
    image = riv_make_image(imagePath.c_str(), alpha);

    // load data json file
    std::ifstream file(dataPath);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + dataPath);
    }

    std::string line;

    // read file line by line
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string key;
        riv_recti entry;

        // each line contains number of moves, board description and cluster count
        if (ss >> key >> entry.x >> entry.y >> entry.width >> entry.height)
        {
            data[key] = entry;
        }
        else
        {
            throw std::runtime_error("Invalid line format in file: " + dataPath);
        }
    }

    file.close();
}

void SpriteSheet::Draw(std::string key, int64_t x0, int64_t y0, int64_t mw, int64_t mh)
{
    riv_recti rect = data[key];
    riv_draw_image_rect(image, x0, y0, rect.width, rect.height, rect.x, rect.y, mw, mh);
}

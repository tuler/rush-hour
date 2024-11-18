#pragma once
#include <map>
#include <string>

extern "C"
{
#include "riv.h"
}

class SpriteSheet
{
public:
    SpriteSheet(std::string imagePath, std::string dataPath, int64_t alpha = 255);

    void Draw(std::string key, int64_t x0, int64_t y0, int64_t mw, int64_t mh);

private:
    // image id loaded using riv
    int image;

    // map from key to rect
    std::map<std::string, riv_recti> data;
};

#ifndef IMAGELOADER_H
#define IMAGELOADER_H
#include "GLTexture.h"
#include <string>

class ImageLoader
{
public:
    static GLTexture load_image(std::string file_path);
};

#endif
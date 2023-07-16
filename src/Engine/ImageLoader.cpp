#include "ImageLoader.h"
#include "stb/stb_image.h"
#include <vector>
#include "IOManager.h"
#include "Errors.h"

GLTexture ImageLoader::load_image(std::string file_path)
{
    GLTexture texture = {};

    int number_of_channels;

    // Load texture
    unsigned char *texture_data = stbi_load(file_path.c_str(), &texture.width, &texture.height, &number_of_channels, STBI_rgb_alpha);
    if(texture_data)
    {
        // Generate and bind texture to the type (GL_TEXTURE_2D)
        glGenTextures(1, &texture.id);
        glBindTexture(GL_TEXTURE_2D, texture.id);
        // Upload texture data
        // Pass GL_RGBA for both parameters to include alpha values in texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
        // Set texture wrapping and filtering options
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE /*GL_REPEAT*/);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE /*GL_REPEAT*/);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // GL_NEAREST fixes texture edge flickering!!!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
        printf("Failed to open texture data");
    }

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(texture_data);

    return texture;

}
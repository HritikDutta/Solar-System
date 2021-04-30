#include "texture.h"

#include <iostream>
#include <glad/glad.h>
#include <stb/stb_image.h>

void Texture::Load(const char* filepath)
{
    int bytesPP;
    stbi_set_flip_vertically_on_load(true);
    u8* pixels = stbi_load(filepath, &width, &height, &bytesPP, 0);

    if (!pixels)
    {
        std::cout << "Failed to load image\n";
        return;
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLint internalFormat;
    GLint format;

    switch (bytesPP)
    {
        case 3:
        {
            internalFormat = GL_RGB8;
            format = GL_RGB;
        } break;

        case 4:
        {
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
        } break;

        default:
            __debugbreak();
            break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(pixels);
}

void Texture::Bind(u32 activeTextureIndex)
{
    glActiveTexture(GL_TEXTURE0 + activeTextureIndex);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Free()
{
    glDeleteTextures(1, &id);
}
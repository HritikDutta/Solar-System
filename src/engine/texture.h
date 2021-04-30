#pragma once

#include "universal/types.h"

struct Texture
{
    u32 id;
    s32 width, height;

    void Load(const char* filepath);
    void Bind(u32 activeTextureIndex);
    void Unbind();
    void Free();
};
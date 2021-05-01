#pragma once

#include "universal/types.h"
#include "shader.h"

struct Mesh
{
    u32 VAO;
    u32 meshIBO;
    u32 meshIndexCount;

    void LoadFromOBJ(const char* meshpath);

    void Bind();
    void Draw();
    void Free();
};
#pragma once

#include "universal/types.h"

struct TexturedCube
{
    f32 side;
    u32 vao;

    static TexturedCube Gen(f32 side);
    void Draw() const;
};
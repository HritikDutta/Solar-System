#include "textured_cube.h"

#include <iostream>
#include <glad/glad.h>

TexturedCube TexturedCube::Gen(f32 side)
{
    f32 vs[] = {
        0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        side, 0.0f, 0.0f,  1.0f, 0.0f,
        side, side, 0.0f,  1.0f, 1.0f,
        side, side, 0.0f,  1.0f, 1.0f,
        0.0f, side, 0.0f,  0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

        0.0f, 0.0f, side,  0.0f, 0.0f,
        side, 0.0f, side,  1.0f, 0.0f,
        side, side, side,  1.0f, 1.0f,
        side, side, side,  1.0f, 1.0f,
        0.0f, side, side,  0.0f, 1.0f,
        0.0f, 0.0f, side,  0.0f, 0.0f,

        0.0f, side, side,  1.0f, 0.0f,
        0.0f, side, 0.0f,  1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        0.0f, 0.0f, side,  0.0f, 0.0f,
        0.0f, side, side,  1.0f, 0.0f,

        side, side, side,  1.0f, 0.0f,
        side, side, 0.0f,  1.0f, 1.0f,
        side, 0.0f, 0.0f,  0.0f, 1.0f,
        side, 0.0f, 0.0f,  0.0f, 1.0f,
        side, 0.0f, side,  0.0f, 0.0f,
        side, side, side,  1.0f, 0.0f,

        0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        side, 0.0f, 0.0f,  1.0f, 1.0f,
        side, 0.0f, side,  1.0f, 0.0f,
        side, 0.0f, side,  1.0f, 0.0f,
        0.0f, 0.0f, side,  0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

        0.0f, side, 0.0f,  0.0f, 1.0f,
        side, side, 0.0f,  1.0f, 1.0f,
        side, side, side,  1.0f, 0.0f,
        side, side, side,  1.0f, 0.0f,
        0.0f, side, side,  0.0f, 0.0f,
        0.0f, side, 0.0f,  0.0f, 1.0f
    };

    u32 vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    u32 vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vs), vs, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(3 * sizeof(f32)));

    return TexturedCube { side, vao };
}

void TexturedCube::Draw() const
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
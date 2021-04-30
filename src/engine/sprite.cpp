#include "sprite.h"

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <stb/stb_image.h>
#include "universal/types.h"
#include "shader.h"

void Sprite::Set(Vec2 pixelSize, Vec4 texCoords)
{
    size = pixelSize;
    uvs  = texCoords;

    f32 x_2 = pixelSize.x / (2.0f * pixelSize.y);

    f32 vs[] = {
        -x_2, -0.5, texCoords.t, texCoords.v,
         x_2, -0.5, texCoords.u, texCoords.v,
         x_2,  0.5, texCoords.u, texCoords.s,
        -x_2,  0.5, texCoords.t, texCoords.s,
    };

    u32 is[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    u32 vbo;
    
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vs), vs, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*) 0);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(is), is, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Sprite::Draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
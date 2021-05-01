#pragma once

#include <glad/glad.h>
#include "universal/types.h"

template <class T>
struct UniformBuffer
{
    u32 id;
    u32 binding;

    UniformBuffer() = default;
    UniformBuffer(u32 binding)
    {
        glGenBuffers(1, &id);
        glBindBuffer(GL_UNIFORM_BUFFER, id);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(T), NULL, GL_STATIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, id, 0, sizeof(T));
    }

    void SetData(const T& data)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, id);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &data);
    }
};
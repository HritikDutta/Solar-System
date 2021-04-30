#pragma once

// Hash functions for all vectors

#include <unordered_map>
#include "vecs/vec2.h"
#include "vecs/vec3.h"
#include "vecs/vec4.h"

namespace std
{
    template<>
    struct hash<Vec2>
    {
        size_t operator()(Vec2 const& vec) const;
    };

    template<>
    struct hash<Vec3>
    {
        size_t operator()(Vec3 const& vec) const;
    };

    template<>
    struct hash<Vec4>
    {
        size_t operator()(Vec4 const& vec) const;
    };
}
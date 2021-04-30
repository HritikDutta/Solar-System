#include "hash.h"

#include <unordered_map>
#include "vecs/vec2.h"
#include "vecs/vec3.h"
#include "vecs/vec4.h"

static void CombineHash(size_t& seed, size_t hash)
{
    // Copied from glm
    hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash;
}

namespace std
{
    size_t hash<Vec2>::operator()(Vec2 const& vec) const
    {
        size_t seed = 0;
        hash<f32> hasher;
        CombineHash(seed, hasher(vec.x));
        CombineHash(seed, hasher(vec.y));
        return seed;
    }

    size_t hash<Vec3>::operator()(Vec3 const& vec) const
    {
        size_t seed = 0;
        hash<f32> hasher;
        CombineHash(seed, hasher(vec.x));
        CombineHash(seed, hasher(vec.y));
        CombineHash(seed, hasher(vec.z));
        return seed;
    }

    size_t hash<Vec4>::operator()(Vec4 const& vec) const
    {
        size_t seed = 0;
        hash<f32> hasher;
        CombineHash(seed, hasher(vec.x));
        CombineHash(seed, hasher(vec.y));
        CombineHash(seed, hasher(vec.z));
        CombineHash(seed, hasher(vec.w));
        return seed;
    }
}
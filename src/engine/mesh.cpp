#include "mesh.h"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <tiny_obj_loader.h>
#include "universal/types.h"
#include "universal/hash.h"
#include "shader.h"

struct Vertex
{
    Vec3 pos;
    Vec3 normal;
    Vec2 texCoord;

    bool operator==(const Vertex& other) const
    {
        return pos == other.pos && normal == other.normal && texCoord == other.texCoord;
    }
};

namespace std
{
    template<>
    struct hash<Vertex>
    {
        size_t operator()(Vertex const& vert) const
        {
            return hash<Vec3>()(vert.pos) ^ hash<Vec3>()(vert.normal) ^ hash<Vec2>()(vert.texCoord);
        }
    };
}

void Mesh::LoadFromOBJ(const char* meshpath)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, meshpath))
    {
        std::cout << warn << " + " << err;
        return;
    }

    std::vector<Vertex> vertices;
    std::vector<u32> meshIndices;

    std::unordered_map<Vertex, u32> uniqueVertices;

    for (const auto& shape : shapes)
    {
        int i = 1;
        for (const auto& index : shape.mesh.indices)
        {
            Vertex v {};

            v.pos = Vec3 {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2],
            };

            v.normal = Vec3 {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2],
            };

            v.texCoord = Vec2 {
                attrib.texcoords[2 * index.texcoord_index + 0],
                attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertices.push_back(v);

            if (uniqueVertices.find(v) == uniqueVertices.end())
            {
                uniqueVertices[v] = (u32) vertices.size();
                vertices.push_back(v);
            }

            u32 idx = uniqueVertices[v];
            meshIndices.push_back(idx);

            i++;
        }
    }

    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    u32 vbo;

    glCreateBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) sizeof(Vec3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (2 * sizeof(Vec3)));

    glBindVertexArray(0);

    glCreateBuffers(1, &meshIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * meshIndices.size(), meshIndices.data(), GL_STATIC_DRAW);

    meshIndexCount = meshIndices.size();
}

void Mesh::Bind()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIBO);
}

void Mesh::Draw()
{
    glDrawElements(GL_TRIANGLES, meshIndexCount, GL_UNSIGNED_INT, NULL);
}

void Mesh::Free()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &meshIBO);
}
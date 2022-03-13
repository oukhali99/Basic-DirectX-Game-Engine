#include "Cube.h"
#include "btBulletDynamicsCommon.h"
#include "Game.h"
#include "ShaderResources.h"

Cube::Cube(GameObject* gameObject)
    :
    ShapeBase(gameObject, 14)
{
    // Create a resource for the vertices
    VERTEX OurVertices[] = {
        // 0
        {
            { 1, 1, -1 },
            { 2.0f / 3.0f   , 1.0f / 4.0f }
        },

        // 1
        {
            { 1, -1, -1 },
            { 2.0f / 3.0f   , 2.0f / 4.0f }
        },

        // 2
        {
            { -1, -1, -1 },
            { 1.0f / 3.0f   , 2.0f / 4.0f }
        },

        // 3
        {
            { -1, 1, -1 },
            { 1.0f / 3.0f   , 1.0f / 4.0f }
        },

        // 4
        {
            { 1, 1, 1 },
            { 2.0f / 3.0f   , 4.0f / 4.0f }
        },


        // 5
        {
             { 1, -1, 1 },
             { 2.0f / 3.0f   , 3.0f / 4.0f }
        },


        // 6
        {
            { -1, -1, 1 },
            { 1.0f / 3.0f   , 3.0f / 4.0f }
        },

        // 7
        {
            { -1, 1, 1 },
            { 1.0f / 3.0f   , 4.0f / 4.0f }
        },

        // 8
        {
            { -1, 1, 1 },
            { 1.0f / 3.0f   , 0.0f / 4.0f }
        },

        //9
        {
            { 1, 1, 1 },
            { 2.0f / 3.0f   , 0.0f / 4.0f }
        },

        // 10
        {
            { -1, 1, 1 },
            { 0.0f / 3.0f   , 1.0f / 4.0f }
        },

        // 11
        {
            { -1, -1, 1 },
            { 0.0f / 3.0f   , 2.0f / 4.0f }
        },

        // 12
        {
            { 1, 1, 1 },
            { 3.0f / 3.0f   , 1.0f / 4.0f }
        },

        // 13
        {
            { 1, -1, 1 },
            { 3.0f / 3.0f   , 2.0f / 4.0f }
        },
    };
    memcpy(vertices, OurVertices, vertexCount * sizeof(VERTEX));

    unsigned short indices[] = {
            0, 1, 2, 3, 0, 2,
            4, 6, 5, 6, 4, 7,
            0, 12, 1, 1, 12, 13,
            3, 11, 10, 3, 2, 11,
            3, 8, 0, 0, 8, 9,
            2, 5, 6, 1, 5, 2,
    };

    if (bindables.size() == 0) {
        VertexBuffer* vertexBuffer = new VertexBuffer(vertexCount);
        TransformBuffer* transformBuffer = new TransformBuffer();
        ColorBuffer* colorBuffer = new ColorBuffer(6);
        IndexBuffer* indexBuffer = new IndexBuffer(indices, sizeof(indices));
        ShaderResources* shaderResources = new ShaderResources(256, 256);

        // ORDER OF LOADING MATTERS
        bindables.push_back(shaderResources);    // THIS MUST BE LOADED FIRST
        bindables.push_back(vertexBuffer);
        bindables.push_back(transformBuffer);
        bindables.push_back(colorBuffer);
        bindables.push_back(indexBuffer);
    }
}

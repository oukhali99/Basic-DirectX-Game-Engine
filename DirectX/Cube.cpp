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
            { 2.0f, 1.0f, 0 }
        },

        // 1
        {
            { 1, -1, -1 },
            { 2.0f, 2.0f, 0 }
        },

        // 2
        {
            { -1, -1, -1 },
            { 1.0f, 2.0, 0 }
        },

        // 3
        {
            { -1, 1, -1 },
            { 1.0f, 1.0f, 0 }
        },

        // 4
        {
            { 1, 1, 1 },
            { 2.0f, 4.0f, 1 }
        },


        // 5
        {
             { 1, -1, 1 },
             { 2.0f, 3.0f, 1 }
        },


        // 6
        {
            { -1, -1, 1 },
            { 1.0f, 3.0f, 1 }
        },

        // 7
        {
            { -1, 1, 1 },
            { 1.0f, 4.0f, 1 }
        },

        // 8
        {
            { -1, 1, 1 },
            { 1.0f, 0.0f, 1 }
        },

        //9
        {
            { 1, 1, 1 },
            { 2.0f, 0.0f, 1 }
        },

        // 10
        {
            { -1, 1, 1 },
            { 0.0f, 1.0f, 1 }
        },

        // 11
        {
            { -1, -1, 1 },
            { 0.0f, 2.0f, 1 }
        },

        // 12
        {
            { 1, 1, 1 },
            { 3.0f, 1.0f, 1 }
        },

        // 13
        {
            { 1, -1, 1 },
            { 3.0f, 2.0f, 1 }
        },
    };
    memcpy(vertices, OurVertices, vertexCount * sizeof(VERTEX));

    unsigned short indices[] = {
        // +x
        0, 12, 1, 1, 12, 13,
        // -x
        3, 11, 10, 3, 2, 11,
        // +y
        3, 8, 0, 0, 8, 9,
        // -y
        2, 5, 6, 1, 5, 2,
        // +z
        4, 6, 5, 6, 4, 7,
        // -z
        0, 1, 2, 3, 0, 2,
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

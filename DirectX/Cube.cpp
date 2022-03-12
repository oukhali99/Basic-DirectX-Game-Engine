#include "Cube.h"
#include "btBulletDynamicsCommon.h"
#include "Game.h"
#include "ShaderResources.h"

Cube::Cube(GameObject* gameObject)
    :
    ShapeBase(gameObject)
{
    // Create a resource for the vertices
    VERTEX OurVertices[] = {
        {
            { 1, 1, -1 },
            { 1, 0 }
        },

        {
            { 1, -1, -1 },
            { 1, 1 }
        },

        {
            { -1, -1, -1 },
            { 0, 1 }
        },

        {
            { -1, 1, -1 },
            { 0, 0 }
        },

        {
            { 1, 1, 1 },
            { 1, 0 }
        },

        {
             { 1, -1, 1 },
             { 1, 1 }
        },

        {
            { -1, -1, 1 },
            { 0, 1 }
        },

        {
            { -1, 1, 1 },
            { 0, 0 }
        },
    };

    unsigned short indices[] = {
            0, 1, 2, 3, 0, 2,
            4, 6, 5, 6, 4, 7,
            0, 4, 1, 1, 4, 5,
            3, 6, 7, 3, 2, 6,
            3, 7, 0, 0, 7, 4,
            2, 5, 6, 1, 5, 2,
    };

    if (bindables.size() == 0) {
        VertexBuffer* vertexBuffer = new VertexBuffer(OurVertices, sizeof(OurVertices));
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

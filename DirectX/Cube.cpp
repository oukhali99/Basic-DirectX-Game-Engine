#include "Cube.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "ShaderResources.h"
#include "TransformConstantBuffer.h"
#include "GameObject.h"
#include "ColorConstantBuffer.h"

Cube::Cube(GameObject* gameObject)
    :
    ShapeBase(gameObject, 24, 36)
{
    SetupVertices();
    SetupIndices();

    if (bindables.size() == 0) {
        VertexBuffer* vertexBuffer = new VertexBuffer(vertexCount);
        TransformConstantBuffer* transformBuffer = new TransformConstantBuffer();
        ColorConstantBuffer* colorBuffer = new ColorConstantBuffer(6);
        IndexBuffer* indexBuffer = new IndexBuffer(indices, indexCount);
        ShaderResources* shaderResources = new ShaderResources(256, 256);

        // ORDER OF LOADING MATTERS
        bindables.push_back(shaderResources);    // THIS MUST BE LOADED FIRST
        bindables.push_back(vertexBuffer);
        bindables.push_back(transformBuffer);
        bindables.push_back(colorBuffer);

        this->indexBuffer = indexBuffer;
    }
}

void Cube::SetupVertices() {
    float sizeX = gameObject->GetScale().x();
    float sizeY = gameObject->GetScale().y();
    float sizeZ = gameObject->GetScale().z();

    // Create a resource for the vertices
    VERTEX OurVertices[] = {
        { { -1.0f, 1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f * sizeX, 0.0f * sizeZ } }, // +Y (top face)
        { { 1.0f, 1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f * sizeX, 0.0f * sizeZ } },
        { { 1.0f, 1.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f * sizeX, 1.0f * sizeZ } },
        { { -1.0f, 1.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f * sizeX, 1.0f * sizeZ } },

        { { -1.0f, -1.0f,  1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f * sizeX, 0.0f * sizeZ } }, // -Y (bottom face)
        { { 1.0f, -1.0f,  1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f * sizeX, 0.0f * sizeZ } },
        { { 1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f * sizeX, 1.0f * sizeZ } },
        { { -1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f * sizeX, 1.0f * sizeZ } },

        { { 1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f * sizeZ, 0.0f * sizeY } }, // +X (right face)
        { { 1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f * sizeZ, 0.0f * sizeY } },
        { { 1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f * sizeZ, 1.0f * sizeY } },
        { { 1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f * sizeZ, 1.0f * sizeY } },

        { { -1.0f,  1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f * sizeZ, 0.0f * sizeY } }, // -X (left face)
        { { -1.0f,  1.0f,  1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f * sizeZ, 0.0f * sizeY } },
        { { -1.0f, -1.0f,  1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f * sizeZ, 1.0f * sizeY } },
        { { -1.0f, -1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f * sizeZ, 1.0f * sizeY } },

        { { -1.0f,  1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f * sizeX, 0.0f * sizeY } }, // +Z (front face)
        { { 1.0f,  1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f * sizeX, 0.0f * sizeY } },
        { { 1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f * sizeX, 1.0f * sizeY } },
        { { -1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f * sizeX, 1.0f * sizeY } },

        { { 1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f * sizeX, 0.0f * sizeY } }, // -Z (back face)
        { { -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f * sizeX, 0.0f * sizeY } },
        { { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f * sizeX, 1.0f * sizeY } },
        { { 1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f * sizeX, 1.0f * sizeY } },
    };
    memcpy(vertices, OurVertices, sizeof(OurVertices));
}

void Cube::SetupIndices() {
    unsigned short indices[] = {
        0, 2, 1,
        0, 3, 2,

        4, 6, 5,
        4, 7, 6,

        8, 10, 9,
        8, 11, 10,

        12, 14, 13,
        12, 15, 14,

        16, 18, 17,
        16, 19, 18,

        20, 22, 21,
        20, 23, 22
    };
    memcpy(this->indices, indices, sizeof(indices));
}

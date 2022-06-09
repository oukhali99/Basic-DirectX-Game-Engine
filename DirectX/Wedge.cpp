#include "Wedge.h"
#include "GameObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderResources.h"
#include "ConstantBuffer.h"
#include "TransformConstantBuffer.h"
#include "ColorConstantBuffer.h"

Wedge::Wedge(GameObject* gameObject) 
	:
	ShapeBase(gameObject, 18, 24)
{
    SetupVertices();
    SetupIndices();

    if (bindables.size() == 0) {
        VertexBuffer* vertexBuffer = new VertexBuffer(vertexCount);
        TransformConstantBuffer* transformBuffer = new TransformConstantBuffer();
        ColorConstantBuffer* colorBuffer = new ColorConstantBuffer(5);
        IndexBuffer* indexBuffer = new IndexBuffer(indices, indexCount);
        ShaderResources* shaderResources = new ShaderResources(256, 256);

        // ORDER OF LOADING MATTERS
        bindables.push_back(vertexBuffer);
        bindables.push_back(transformBuffer);
        bindables.push_back(colorBuffer);
        bindables.push_back(shaderResources);
        bindables.push_back(indexBuffer);
    }
}

void Wedge::SetupVertices() {
    float sizeX = gameObject->GetScale().x();
    float sizeY = gameObject->GetScale().y();
    float sizeZ = gameObject->GetScale().z();

    // Create a resource for the vertices
    VERTEX OurVertices[] = {
        { { -1.0f, -1.0f,  1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f * sizeX, 0.0f * sizeZ } }, // -Y (bottom face)
        { { 1.0f, -1.0f,  1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f * sizeX, 0.0f * sizeZ } },
        { { 1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f * sizeX, 1.0f * sizeZ } },
        { { -1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f * sizeX, 1.0f * sizeZ } },

        { { 1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f * sizeZ, 0.0f * sizeY } }, // +X (right face)
        { { 1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f * sizeZ, 1.0f * sizeY } },
        { { 1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f * sizeZ, 1.0f * sizeY } },

        { { -1.0f,  1.0f,  1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f * sizeZ, 0.0f * sizeY } }, // -X (left face)
        { { -1.0f, -1.0f,  1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f * sizeZ, 1.0f * sizeY } },
        { { -1.0f, -1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f * sizeZ, 1.0f * sizeY } },

        { { -1.0f,  1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f * sizeX, 0.0f * sizeY } }, // +Z (front face)
        { { 1.0f,  1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f * sizeX, 0.0f * sizeY } },
        { { 1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f * sizeX, 1.0f * sizeY } },
        { { -1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f * sizeX, 1.0f * sizeY } },

        { { 1.0f,  1.0f, 1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f * sizeX, 0.0f * sizeY } }, // -Z (back face)
        { { -1.0f,  1.0f, 1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f * sizeX, 0.0f * sizeY } },
        { { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f * sizeX, 1.0f * sizeY } },
        { { 1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f * sizeX, 1.0f * sizeY } },
    };
    memcpy(vertices, OurVertices, vertexCount * sizeof(VERTEX));
}

void Wedge::SetupIndices() {
    unsigned short indices[] = {
        0, 2, 1,
        0, 3, 2,

        4, 6, 5,

        7, 9, 8,

        10, 12, 11,
        10, 13, 12,

        14, 16, 15,
        14, 17, 16
    };
    memcpy(this->indices, indices, sizeof(indices));
}

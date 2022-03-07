#include "Cube.h"

Cube::Cube(Graphics& gfx, Transform transform)
    :
    Shape(gfx, transform)
{
    // Create a resource for the vertices
    VERTEX OurVertices[] = {
        {1.0f, 1.0f, -1.0f},

        {1.0f, -1.0f, -1.0f},

        {-1.0f, -1.0f, -1.0f},

        {-1.0f, 1.0f, -1.0f},

        {1.0f, 1.0f, 1.0f},

        {1.0f, -1.0f, 1.0f},

        {-1.0f, -1.0f, 1.0f},

        {-1.0f, 1.0f, 1.0f},
    };

    // Create the face color resource
    FaceColors fc = {
        {
            { 1.0f, 0.0f, 0.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f, 1.0f },
            { 1.0f, 0.0f, 1.0f, 1.0f },
            { 1.0f, 1.0f, 0.0f, 1.0f },
        }
    };

    unsigned short indices[] = {
            0, 1, 2, 3, 0, 2,
            4, 6, 5, 6, 4, 7,
            0, 4, 1, 1, 4, 5,
            3, 6, 7, 3, 2, 6,
            3, 7, 0, 0, 7, 4,
            2, 5, 6, 1, 5, 2,
    };

    VertexBuffer* vb = new VertexBuffer(gfx, OurVertices, sizeof(OurVertices));
    bindables.push_back(vb);

    TransformConstantBuffer* tcb = new TransformConstantBuffer(gfx);
    bindables.push_back(tcb);

    ColorConstantBuffer* ccb = new ColorConstantBuffer(gfx, fc);
    bindables.push_back(ccb);

    IndexBuffer* ib = new IndexBuffer(gfx, indices, sizeof(indices));
    bindables.push_back(ib);
}

void Cube::RenderFrame() {
    // Get the time
    float t = Clock::GetSingleton().GetTimeSinceStart();
    transform.xRot = 2 * t;
    transform.yRot = 3 * t;
    transform.zRot = 1 * t;

    for (Bindable* bindable : bindables) {
        bindable->Bind(transform);
    }

    // draw the vertex buffer to the back buffer
    GetDeviceContext(gfx)->DrawIndexed(36u, 0u, 0u);
}

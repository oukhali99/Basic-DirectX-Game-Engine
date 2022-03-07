#include "Cube.h"

Cube::Cube(ID3D11Device& pDevice, ID3D11DeviceContext& pContext, Transform transform)
    :
    Shape(pDevice, pContext, transform)
{
    InitGraphics();
}

Cube::~Cube() {
    pVBuffer->Release();
    pIBuffer->Release();
    pCTransformationBuffer->Release();
    pCFaceColorBuffer->Release();
}

void Cube::InitGraphics() {
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

    VertexBuffer* vb = new VertexBuffer(&pContext, &pDevice, OurVertices, sizeof(OurVertices));
    bindables.push_back(vb);

    TransformConstantBuffer* tcb = new TransformConstantBuffer(&pContext, &pDevice);
    bindables.push_back(tcb);

    ColorConstantBuffer* ccb = new ColorConstantBuffer(&pContext, &pDevice, fc);
    bindables.push_back(ccb);

    IndexBuffer* ib = new IndexBuffer(&pContext, &pDevice, indices, sizeof(indices));
    bindables.push_back(ib);

    // select which primtive type we are using
    pContext.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
    pContext.DrawIndexed(36u, 0u, 0u);
}

void Cube::OnButtonPressed(WPARAM wParam) {
    float increment = 0.1f;

    if (wParam == 'w') {
        transform.y += increment;
    }
    else if (wParam == 's') {
        transform.y -= increment;
    }
    else if (wParam == 'd') {
        transform.x += increment;
    }
    else if (wParam == 'a') {
        transform.x -= increment;
    }
}

void Cube::OnMouseMovedTo(Mouse::Position position) {
    transform.x = 6 * position.x;
    transform.z = 20 * position.y;
}

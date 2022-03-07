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

    VertexBuffer* vb = new VertexBuffer(&pContext, &pDevice, OurVertices, sizeof(OurVertices));
    bindables.push_back(vb);

    TransformConstantBuffer* tcb = new TransformConstantBuffer(&pContext, &pDevice);
    bindables.push_back(tcb);

    // Create the rotation constant buffer
    D3D11_BUFFER_DESC bd;
    

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
    D3D11_SUBRESOURCE_DATA rd;
    ZeroMemory(&rd, sizeof(rd));
    rd.pSysMem = &fc;

    // Create the face color buffer
    ZeroMemory(&bd, sizeof(bd));
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(FaceColors);
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    GFX_THROW_INFO(pDevice.CreateBuffer(&bd, &rd, &pCFaceColorBuffer));

    // Create index resource
    unsigned short indices[] = {
            0, 1, 2, 3, 0, 2,
            4, 6, 5, 6, 4, 7,
            0, 4, 1, 1, 4, 5,
            3, 6, 7, 3, 2, 6,
            3, 7, 0, 0, 7, 4,
            2, 5, 6, 1, 5, 2,
    };
    ZeroMemory(&rd, sizeof(rd));
    rd.pSysMem = indices;

    // Create the index buffer
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(indices);
    bd.StructureByteStride = sizeof(unsigned short);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    GFX_THROW_INFO(pDevice.CreateBuffer(&bd, &rd, &pIBuffer));

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

    // select which buffers to use
    UINT stride = sizeof(VERTEX);
    UINT offset = 0u;
    pContext.IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R16_UINT, 0u);
    pContext.PSSetConstantBuffers(0, 1u, &pCFaceColorBuffer);

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

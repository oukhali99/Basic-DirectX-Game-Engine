#include "Cube.h"

Cube::Cube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :
    pDevice(pDevice),
    pContext(pContext)
{
	InitGraphics();
}

Cube::~Cube() {
    // Unmap from the resources
    pContext->Unmap(pCTransformationBuffer, 0u);

    pVBuffer->Release();
    pIBuffer->Release();
    pCTransformationBuffer->Release();
    pCFaceColorBuffer->Release();
}

void Cube::InitGraphics() {
    // Create a resource for the vertices
    const VERTEX OurVertices[] = {
        // PYRAMID VERTICES
        /*
        {0.0f, 1.0f, 0.0f,
        255u, 0u, 0u, 255u},

        {0.5f, -0.5, 0.0f,
        0u, 255u, 0u, 255u},

        {-0.5f, -0.5f, 0.0f,
        0u, 0u, 255u, 255u},

        {0.0f, 0.0f, 0.6f,
        255u, 255u, 255u, 255u},
        */

        // CUBE VERTICES
        {1.0f, 1.0f, -1.0f},

        {1.0f, -1.0f, -1.0f},

        {-1.0f, -1.0f, -1.0f},

        {-1.0f, 1.0f, -1.0f},

        {1.0f, 1.0f, 1.0f},

        {1.0f, -1.0f, 1.0f},

        {-1.0f, -1.0f, 1.0f},

        {-1.0f, 1.0f, 1.0f},
    };
    D3D11_SUBRESOURCE_DATA rd;
    ZeroMemory(&rd, sizeof(rd));
    rd.pSysMem = OurVertices;

    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(OurVertices);             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &rd, &pVBuffer));       // create the buffer

    // Create the rotation constant buffer
    ZeroMemory(&bd, sizeof(bd));
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, NULL, &pCTransformationBuffer));

    // Create the face color resource
    FaceColors fc = {
        {
            { 1.0f, 0.0f, 0.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f, 1.0f },
            { 0.0f, 1.0f, 1.0f, 1.0f },
            { 1.0f, 0.0f, 1.0f, 1.0f },
            { 1.0f, 1.0f, 0.0f, 1.0f },
        }
    };
    ZeroMemory(&rd, sizeof(rd));
    rd.pSysMem = &fc;

    // Create the face color buffer
    ZeroMemory(&bd, sizeof(bd));
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(FaceColors);
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &rd, &pCFaceColorBuffer));

    // Create index resource
    unsigned short indices[] = {
            0, 1, 2, 3, 0, 2,
            4, 6, 5, 6, 4, 7,
            0, 4, 1, 1, 4, 5,
            3, 6, 7, 3, 2, 6,
            3, 7, 0, 0, 7, 4,
            2, 5, 6, 1, 5, 2,
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
    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &rd, &pIBuffer));

    // Map the constant buffer onto our mapped constant buffer
    ZeroMemory(&mCBuffer, sizeof(D3D11_MAPPED_SUBRESOURCE));
    pContext->Map(pCTransformationBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mCBuffer);

    // select which primtive type we are using
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Cube::RenderFrame(float angle, float xTranslation, float yTranslation, float xRot, float yRot, float zRot) {
    // select which buffers to use
    UINT stride = sizeof(VERTEX);
    UINT offset = 0u;
    pContext->IASetVertexBuffers(0, 1u, &pVBuffer, &stride, &offset);
    pContext->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R16_UINT, 0u);
    pContext->VSSetConstantBuffers(0, 1u, &pCTransformationBuffer);
    pContext->PSSetConstantBuffers(0, 1u, &pCFaceColorBuffer);

    // Create the rotation constant
    float squeeze = (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH;
    const ConstantBuffer cb = {
        dx::XMMatrixTranspose(
            dx::XMMatrixRotationX(xRot * angle) *
            dx::XMMatrixRotationY(yRot * angle) *
            dx::XMMatrixRotationZ(zRot * angle) *
            dx::XMMatrixTranslation(xTranslation, yTranslation, 7.0f) *
            dx::XMMatrixPerspectiveLH(1.0f, squeeze, 0.5f, 10.0f)
        )
    };

    // Copy the data onto the map
    memcpy(mCBuffer.pData, &cb, sizeof(cb));

    // draw the vertex buffer to the back buffer
    pContext->DrawIndexed(36u, 0u, 0u);
}

void Cube::HandleError(HRESULT hr, const char* file, const long long line) {
    std::stringstream ss;

    // Exception line
    ss << "Exception on line " << line << std::endl;

    // Exception file
    ss << "File: " << file << std::endl;

    // Error code
    ss << "Error code: " << "0x" << std::hex << hr << std::endl;

    // Error description
    char errorMessage[50];
    FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        errorMessage,
        sizeof(errorMessage) / sizeof(errorMessage[0]),
        NULL
    );

    ss << "Description: " << errorMessage << std::endl;

    throw new std::exception(ss.str().c_str());
}

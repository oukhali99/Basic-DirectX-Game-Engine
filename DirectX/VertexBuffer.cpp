#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(ID3D11DeviceContext* pContext, ID3D11Device* pDevice, VERTEX* vertices, int verticesSize)
    :
    Bindable(pContext, pDevice)
{
    D3D11_SUBRESOURCE_DATA rd;
    ZeroMemory(&rd, sizeof(rd));
    rd.pSysMem = vertices;

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;                                     // write access access by CPU and GPU
    bd.ByteWidth = verticesSize;                                        // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;                            // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;                         // allow CPU to write in buffer
    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &rd, &pBuffer));          // create the buffer
}

void VertexBuffer::Bind(Transform transform) {
    UINT stride = sizeof(VERTEX);
    UINT offset = 0u;
    pContext->IASetVertexBuffers(0, 1u, &pBuffer, &stride, &offset);
}

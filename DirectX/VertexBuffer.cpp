#include "VertexBuffer.h"
#include "Graphics.h"

VertexBuffer::VertexBuffer(VERTEX* vertices, int verticesSize) {
    D3D11_SUBRESOURCE_DATA rd;
    ZeroMemory(&rd, sizeof(rd));
    rd.pSysMem = vertices;

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;                                     // write access access by CPU and GPU
    bd.ByteWidth = verticesSize;                                        // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;                            // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;                         // allow CPU to write in buffer
    GFX_THROW_INFO(GetDevice(Graphics::GetInstance())->CreateBuffer(&bd, &rd, &pBuffer));          // create the buffer
}

void VertexBuffer::Bind(Shape* shape) {
    UINT stride = sizeof(VERTEX);
    UINT offset = 0u;
    GetDeviceContext(Graphics::GetInstance())->IASetVertexBuffers(0, 1u, &pBuffer, &stride, &offset);
}

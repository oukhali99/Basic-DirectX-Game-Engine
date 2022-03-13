#include "VertexBuffer.h"
#include "Graphics.h"

VertexBuffer::VertexBuffer(int vertexCount) {
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;                                     // write access access by CPU and GPU
    bd.ByteWidth = vertexCount * sizeof(VERTEX);                        // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;                            // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;                         // allow CPU to write in buffer
    GFX_THROW_INFO(GetDevice(Graphics::GetInstance())->CreateBuffer(&bd, NULL, &pBuffer));          // create the buffer
}

void VertexBuffer::Bind(Shape* shape) {
    D3D11_MAPPED_SUBRESOURCE msr = {};
    GFX_THROW_INFO(Graphics::GetInstance()->GetDeviceContext()->Map(pBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr));
    memcpy(msr.pData, shape->GetVertices(), shape->GetVertexCount() * sizeof(VERTEX));
    Graphics::GetInstance()->GetDeviceContext()->Unmap(pBuffer, 0u);

    UINT stride = sizeof(VERTEX);
    UINT offset = 0u;
    GetDeviceContext(Graphics::GetInstance())->IASetVertexBuffers(0, 1u, &pBuffer, &stride, &offset);
}

#include "IndexBuffer.h"
#include "btBulletDynamicsCommon.h"
#include "Graphics.h"
#include "Shape.h"

IndexBuffer::IndexBuffer(unsigned short indices[], UINT sizeOfIndices) 
	:
    Bindable(),
    indices(indices),
    sizeOfIndices(sizeOfIndices)
{
    // Create index resource
    D3D11_SUBRESOURCE_DATA rd;
    ZeroMemory(&rd, sizeof(rd));
    rd.pSysMem = indices;

    // Create the index buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeOfIndices;
    bd.StructureByteStride = sizeof(unsigned short);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreateBuffer(&bd, &rd, &pBuffer));
}

void IndexBuffer::Bind(Shape* shape) {
    // select which buffers to use
    Graphics::GetInstance()->GetDeviceContext()->IASetIndexBuffer(pBuffer, DXGI_FORMAT_R16_UINT, 0u);

    if (sizeOfIndices > 0) {
        // draw the vertex buffer to the back buffer
        Graphics::GetInstance()->GetDeviceContext()->DrawIndexed(sizeOfIndices / sizeof(indices[0]), 0u, 0u);
    }
}

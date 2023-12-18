#include "IndexBuffer.h"
#include <bullet/btBulletDynamicsCommon.h>
#include "Graphics.h"
#include "Shape.h"

IndexBuffer::IndexBuffer(unsigned short* indices, int indexCount)
	:
    Bindable(),
    indexCount(indexCount)
{
    // Create index resource
    D3D11_SUBRESOURCE_DATA rd;
    ZeroMemory(&rd, sizeof(rd));
    rd.pSysMem = indices;

    // Create the index buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(unsigned short) * indexCount;
    bd.StructureByteStride = sizeof(unsigned short);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreateBuffer(&bd, &rd, &pBuffer));
}

void IndexBuffer::Bind(Shape* shape) {
    // select which buffers to use
    Graphics::GetInstance()->GetDeviceContext()->IASetIndexBuffer(pBuffer, DXGI_FORMAT_R16_UINT, 0u);

    int sizeOfIndices = sizeof(unsigned short) * indexCount;
    if (sizeOfIndices > 0) {
        // draw the vertex buffer to the back buffer
        Graphics::GetInstance()->GetDeviceContext()->DrawIndexed(indexCount, 0u, 0u);
    }
}

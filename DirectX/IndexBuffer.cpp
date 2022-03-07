#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(ID3D11DeviceContext* pContext, ID3D11Device* pDevice, unsigned short indices[], UINT sizeOfIndices) 
	:
	Bindable(pContext, pDevice)
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
    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &rd, &pBuffer));
}

void IndexBuffer::Bind(Transform transform) {
    // select which buffers to use
    pContext->IASetIndexBuffer(pBuffer, DXGI_FORMAT_R16_UINT, 0u);

}

#include "ConstantBuffer.h"
#include "Graphics.h"

ConstantBuffer::ConstantBuffer(UINT slotNumber, size_t size)
    :
    slotNumber(slotNumber),
    size(size)
{
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = size;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreateBuffer(&bd, NULL, &pBuffer));
}

void ConstantBuffer::Bind(Shape* shape) {
    D3D11_MAPPED_SUBRESOURCE msr = {};
    GFX_THROW_INFO(Graphics::GetInstance()->GetDeviceContext()->Map(
        pBuffer,
        0u,
        D3D11_MAP_WRITE_DISCARD,
        0u,
        &msr
    ));

    const void* data = shape->GetConstantBufferData(slotNumber);
    if (data) {
        memcpy(msr.pData, data, size);
    }
    else {
        memset(msr.pData, 0, size);
    }
    Graphics::GetInstance()->GetDeviceContext()->Unmap(pBuffer, 0u);

    Graphics::GetInstance()->GetDeviceContext()->VSSetConstantBuffers(slotNumber, 1u, &pBuffer);
    Graphics::GetInstance()->GetDeviceContext()->PSSetConstantBuffers(slotNumber, 1u, &pBuffer);
}

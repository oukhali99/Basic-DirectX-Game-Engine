#include "ConstantBuffer.h"
#include "Graphics.h"

ConstantBuffer::ConstantBuffer(size_t bufferSize) {
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = bufferSize;
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

    const void* data = GetBufferData(shape);
    if (data) {
        memcpy(msr.pData, data, GetBufferSize());
    }
    else {
        memset(msr.pData, 0, GetBufferSize());
    }
    Graphics::GetInstance()->GetDeviceContext()->Unmap(pBuffer, 0u);

    Graphics::GetInstance()->GetDeviceContext()->VSSetConstantBuffers(GetSlotNumber(), 1u, &pBuffer);
    Graphics::GetInstance()->GetDeviceContext()->PSSetConstantBuffers(GetSlotNumber(), 1u, &pBuffer);
}

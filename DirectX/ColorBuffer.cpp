#include "ColorBuffer.h"
#include "Shape.h"
#include "Graphics.h"

ColorBuffer::ColorBuffer(unsigned short faceCount) 
    :
    faceCount(faceCount)
{
    // Create the face color buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(FaceColor) * faceCount;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreateBuffer(&bd, NULL, &pBuffer));
}

void ColorBuffer::Bind(Shape* shape) {
    ID3D11Buffer* bufferToUse = NULL;

    D3D11_MAPPED_SUBRESOURCE msr = {};
    GFX_THROW_INFO(Graphics::GetInstance()->GetDeviceContext()->Map(
        pBuffer,
        0u,
        D3D11_MAP_WRITE_DISCARD,
        0u,
        &msr
    ));

    FaceColor* fc = shape->GetFaceColors();
    if (fc) {
        memcpy(msr.pData, fc, sizeof(FaceColor) * faceCount);
        bufferToUse = pBuffer;
    }

    Graphics::GetInstance()->GetDeviceContext()->Unmap(pBuffer, 0u);

    UINT stride = sizeof(VERTEX);
    UINT offset = 0u;
    //Graphics::GetInstance()->GetDeviceContext()->VSSetConstantBuffers(1, 1u, &bufferToUse);
    Graphics::GetInstance()->GetDeviceContext()->PSSetConstantBuffers(1, 1u, &bufferToUse);
}

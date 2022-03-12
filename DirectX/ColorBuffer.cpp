#include "ColorBuffer.h"
#include "Shape.h"

ColorBuffer::ColorBuffer(unsigned short faceCount) {
    // Create the face color buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(FaceColor) * faceCount;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    GFX_THROW_INFO(GetDevice(Graphics::GetInstance())->CreateBuffer(&bd, NULL, &pBuffer));
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

    FaceColors* fc = shape->GetFaceColors();
    if (fc) {
        memcpy(msr.pData, fc->data, sizeof(FaceColor) * fc->count);
        bufferToUse = pBuffer;
    }

    Graphics::GetInstance()->GetDeviceContext()->Unmap(pBuffer, 0u);

    UINT stride = sizeof(VERTEX);
    UINT offset = 0u;
    GetDeviceContext(Graphics::GetInstance())->PSSetConstantBuffers(0, 1u, &bufferToUse);
}

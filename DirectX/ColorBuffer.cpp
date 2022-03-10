#include "ColorBuffer.h"
#include "Shape.h"

ColorBuffer::ColorBuffer(FaceColors fc) {
    // Create the rotation constant buffer
    D3D11_BUFFER_DESC bd;
    D3D11_SUBRESOURCE_DATA rd;
    ZeroMemory(&rd, sizeof(rd));
    rd.pSysMem = &fc;

    // Create the face color buffer
    ZeroMemory(&bd, sizeof(bd));
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(FaceColors);
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    GFX_THROW_INFO(GetDevice(Graphics::GetInstance())->CreateBuffer(&bd, &rd, &pBuffer));
}

void ColorBuffer::Bind(Shape* shape) {
    UINT stride = sizeof(VERTEX);
    UINT offset = 0u;
    GetDeviceContext(Graphics::GetInstance())->PSSetConstantBuffers(0, 1u, &pBuffer);
}

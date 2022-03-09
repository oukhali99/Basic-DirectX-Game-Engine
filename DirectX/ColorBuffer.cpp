#include "ColorBuffer.h"

ColorBuffer::ColorBuffer(Graphics* gfx, FaceColors fc)
	:
	Bindable(gfx)
{
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
    GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &rd, &pBuffer));
}

void ColorBuffer::Bind(btTransform transform) {
    UINT stride = sizeof(VERTEX);
    UINT offset = 0u;
    GetDeviceContext(gfx)->PSSetConstantBuffers(0, 1u, &pBuffer);
}

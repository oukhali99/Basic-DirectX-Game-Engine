#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx)
	:
	Bindable(gfx)
{
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, NULL, &pBuffer));
}

void TransformConstantBuffer::Bind(btTransform transform) {
    GetDeviceContext(gfx)->VSSetConstantBuffers(0, 1u, &pBuffer);

    float squeeze = (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH;
    const ConstantBuffer cb = {
        dx::XMMatrixTranspose(
            dx::XMMatrixRotationX(transform.getRotation().x()) *
            dx::XMMatrixRotationY(transform.getRotation().y()) *
            dx::XMMatrixRotationZ(transform.getRotation().z()) *
            dx::XMMatrixTranslation(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z()) *
            dx::XMMatrixPerspectiveLH(1.0f, squeeze, 0.5f, 20.0f)
        )
    };

    // Map the constant buffer onto our mapped constant buffer
    D3D11_MAPPED_SUBRESOURCE mCBuffer;
    ZeroMemory(&mCBuffer, sizeof(D3D11_MAPPED_SUBRESOURCE));
    GetDeviceContext(gfx)->Map(pBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mCBuffer);

    // Copy the data onto the map
    memcpy(mCBuffer.pData, &cb, sizeof(cb));

    // Unmap from the resources
    GetDeviceContext(gfx)->Unmap(pBuffer, 0u);
}

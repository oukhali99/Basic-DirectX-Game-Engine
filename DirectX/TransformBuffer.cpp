#include "TransformBuffer.h"
#include "Graphics.h"
#include "Shape.h"
#include "Game.h"

TransformBuffer::TransformBuffer() {
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreateBuffer(&bd, NULL, &pBuffer));
}

void TransformBuffer::Bind(Shape* shape) {
    btTransform transform = shape->GetTransform();
    btVector3 size = shape->GetScale();
    btTransform cameraTransform = Game::GetInstance()->GetMainCamera()->GetGameObject()->GetTransform();

    Graphics::GetInstance()->GetDeviceContext()->VSSetConstantBuffers(0, 1u, &pBuffer);

    float squeeze = (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH;
    dx::XMFLOAT4 quaternionFloat((float)transform.getRotation().x(), (float)transform.getRotation().y(), (float)transform.getRotation().z(), (float)transform.getRotation().w());
    dx::XMVECTOR quaternion = dx::XMLoadFloat4(&quaternionFloat);
    const ConstantBuffer cb = {
        dx::XMMatrixTranspose(
            // Object transform
            dx::XMMatrixScaling(size.x(), size.y(), size.z()) *
            dx::XMMatrixRotationQuaternion(quaternion) *
            dx::XMMatrixTranslation(transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z()) *

            // Camera
            dx::XMMatrixTranslation(-cameraTransform.getOrigin().x(), -cameraTransform.getOrigin().y(), -cameraTransform.getOrigin().z()) *

            // Projection
            dx::XMMatrixPerspectiveLH(1.0f, squeeze, Graphics::GetInstance()->GetNearZ() , Graphics::GetInstance()->GetFarZ())
        )
    };

    // Map the constant buffer onto our mapped constant buffer
    D3D11_MAPPED_SUBRESOURCE mCBuffer;
    ZeroMemory(&mCBuffer, sizeof(D3D11_MAPPED_SUBRESOURCE));
    Graphics::GetInstance()->GetDeviceContext()->Map(pBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mCBuffer);

    // Copy the data onto the map
    memcpy(mCBuffer.pData, &cb, sizeof(cb));

    // Unmap from the resources
    Graphics::GetInstance()->GetDeviceContext()->Unmap(pBuffer, 0u);
}

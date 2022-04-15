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
    btTransform shapeTransform = shape->GetTransform();
    btVector3 shapeSize = shape->GetScale();
    btTransform cameraTransform = Game::GetInstance()->GetMainCamera()->GetGameObject()->GetTransform();

    Graphics::GetInstance()->GetDeviceContext()->VSSetConstantBuffers(0, 1u, &pBuffer);

    dx::XMFLOAT4 shapeQuaternionFloat((float)shapeTransform.getRotation().x(), (float)shapeTransform.getRotation().y(), (float)shapeTransform.getRotation().z(), (float)shapeTransform.getRotation().w());
    dx::XMVECTOR shapeQuaternion = dx::XMLoadFloat4(&shapeQuaternionFloat);

    dx::XMFLOAT4 cameraQuaternionFloat(cameraTransform.getRotation().x(), cameraTransform.getRotation().y(), cameraTransform.getRotation().z(), cameraTransform.getRotation().w());
    dx::XMVECTOR cameraQuaternion = dx::XMLoadFloat4(&cameraQuaternionFloat);

    float squeeze = (float)SCREEN_HEIGHT / (float)SCREEN_WIDTH;
    const ConstantBuffer cb = {
        dx::XMMatrixTranspose(
            // Object transform
            dx::XMMatrixScaling(shapeSize.x(), shapeSize.y(), shapeSize.z()) *
            dx::XMMatrixRotationQuaternion(shapeQuaternion) *
            dx::XMMatrixTranslation(shapeTransform.getOrigin().x(), shapeTransform.getOrigin().y(), shapeTransform.getOrigin().z()) *

            // Camera
            dx::XMMatrixTranslation(-cameraTransform.getOrigin().x(), -cameraTransform.getOrigin().y(), -cameraTransform.getOrigin().z()) *
            dx::XMMatrixRotationQuaternion(cameraQuaternion) *

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

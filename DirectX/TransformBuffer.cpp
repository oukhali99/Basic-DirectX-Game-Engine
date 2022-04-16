#include "TransformBuffer.h"
#include "Graphics.h"
#include "Shape.h"
#include "Game.h"
#include "Window.h"

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

    Graphics::GetInstance()->GetDeviceContext()->VSSetConstantBuffers(0, 1u, &pBuffer);

    dx::XMVECTOR shapeQuaternion = dx::XMVectorSet((float)shapeTransform.getRotation().x(), (float)shapeTransform.getRotation().y(), (float)shapeTransform.getRotation().z(), (float)shapeTransform.getRotation().w());

    RECT clientRect;
    GetClientRect(Window::GetInstance()->GetHandle(), &clientRect);
    float squeeze = (float)clientRect.bottom / (float)clientRect.right;

    const ConstantBuffer cb = {
        dx::XMMatrixTranspose(
            // Object transform
            dx::XMMatrixScaling(shapeSize.x(), shapeSize.y(), shapeSize.z()) *
            dx::XMMatrixRotationQuaternion(shapeQuaternion) *
            dx::XMMatrixTranslation(shapeTransform.getOrigin().x(), shapeTransform.getOrigin().y(), shapeTransform.getOrigin().z()) *

            // Camera
            Game::GetInstance()->GetMainCamera()->GetMatrix() *

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

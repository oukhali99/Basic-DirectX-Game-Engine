#include "TransformConstantBuffer.h"
#include "Shape.h"
#include "Window.h"
#include "Graphics.h"
#include "Game.h"

TransformConstantBuffer::TransformConstantBuffer()
	:
	ConstantBuffer(GetBufferSize())
{
    this->data = new Data();
}

TransformConstantBuffer::~TransformConstantBuffer() {
    delete this->data;
}

UINT TransformConstantBuffer::GetSlotNumber() {
	return 0u;
}

size_t TransformConstantBuffer::GetBufferSize() {
	return sizeof(Data);
}

const void* TransformConstantBuffer::GetBufferData(Shape* shape) {
    btTransform shapeTransform = shape->GetTransform();
    btVector3 shapeSize = shape->GetScale();

    dx::XMVECTOR shapeQuaternion = dx::XMVectorSet((float)shapeTransform.getRotation().x(), (float)shapeTransform.getRotation().y(), (float)shapeTransform.getRotation().z(), (float)shapeTransform.getRotation().w());

    RECT clientRect;
    GetClientRect(Window::GetInstance()->GetHandle(), &clientRect);
    float squeeze = (float)clientRect.bottom / (float)clientRect.right;

    dx::XMMATRIX worldTransformation =
        dx::XMMatrixScaling(shapeSize.x(), shapeSize.y(), shapeSize.z()) *
        dx::XMMatrixRotationQuaternion(shapeQuaternion) *
        dx::XMMatrixTranslation(shapeTransform.getOrigin().x(), shapeTransform.getOrigin().y(), shapeTransform.getOrigin().z())
        ;

    dx::XMMATRIX viewTransformation =
        Game::GetInstance()->GetMainCamera()->GetMatrix()
        ;

    dx::XMMATRIX projectionTransformation =
        dx::XMMatrixPerspectiveLH(1.0f, squeeze, Graphics::GetInstance()->GetNearZ(), Graphics::GetInstance()->GetFarZ())
        ;


    worldTransformation = dx::XMMatrixTranspose(worldTransformation);
    viewTransformation = dx::XMMatrixTranspose(viewTransformation);
    projectionTransformation = dx::XMMatrixTranspose(projectionTransformation);

    /*
    Data data = {
        // Object transform
        worldTransformation,
        viewTransformation,
        projectionTransformation
    };
    */

    data->worldTransformation = worldTransformation;
    data->viewTransformation = viewTransformation;
    data->projectionTransformation = projectionTransformation;

    return data;
}

#include "Shape.h"
#include "Graphics.h"
#include "btBulletDynamicsCommon.h"
#include "Component.h"
#include "GameObject.h"
#include "Texture.h"
#include "Window.h"
#include "TransformBuffer.h"
#include "Game.h"

Shape::Shape(GameObject* gameObject, int vertexCount)
	:
    Component(gameObject),
	hr(0),
    texture(0),
    faceColors(0),
    vertexCount(vertexCount)
{
    vertices = (VERTEX*)malloc(vertexCount * sizeof(VERTEX));
}

Shape::~Shape() {
    free(vertices);
}

btTransform Shape::GetTransform() {
    return gameObject->GetTransform();
}

btVector3 Shape::GetScale() {
    return gameObject->GetScale();
}

Texture* Shape::GetTexture() {
    return texture;
}

FaceColor* Shape::GetFaceColors() {
    return faceColors;
}

VERTEX* Shape::GetVertices() {
    return vertices;
}

int Shape::GetVertexCount() {
    return vertexCount;
}

const void* Shape::GetConstantBufferData(UINT slotNumber) {
    switch (slotNumber) {
        case 0u: {
            btTransform shapeTransform = GetTransform();
            btVector3 shapeSize = GetScale();

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
                worldTransformation *
                Game::GetInstance()->GetMainCamera()->GetMatrix() *
                dx::XMMatrixPerspectiveLH(1.0f, squeeze, Graphics::GetInstance()->GetNearZ(), Graphics::GetInstance()->GetFarZ())
                ;

            worldTransformation = dx::XMMatrixTranspose(worldTransformation);
            viewTransformation = dx::XMMatrixTranspose(viewTransformation);

            TransformBuffer::ConstantBuffer buffer = {
                // Object transform
                worldTransformation,
                viewTransformation
            };

            return &buffer;
        }
        case 1u: {
            return faceColors;
        }
    }

    return 0;
}

void Shape::SetTexture(Texture* texture) {
    this->texture = texture;
}

void Shape::SetFaceColors(FaceColor* pFaceColors) {
    this->faceColors = pFaceColors;
}

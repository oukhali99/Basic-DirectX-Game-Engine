#include "Shape.h"
#include "Graphics.h"
#include "btBulletDynamicsCommon.h"
#include "Component.h"
#include "GameObject.h"

Shape::Shape(GameObject* gameObject)
	:
    Component(gameObject),
	hr(0),
    texturePath(""),
    faceColors(0)
{}

btTransform Shape::GetTransform() {
    return gameObject->GetTransform();
}

btVector3 Shape::GetScale() {
    return gameObject->GetScale();
}

std::string Shape::GetTexturePath() {
    return texturePath;
}

FaceColors* Shape::GetFaceColors() {
    return faceColors;
}

void Shape::SetTexturePath(std::string texturePath) {
    this->texturePath = std::string(texturePath);
}

void Shape::SetFaceColors(FaceColors* pFaceColors) {
    this->faceColors = pFaceColors;
}

#include "Shape.h"
#include "Graphics.h"
#include "btBulletDynamicsCommon.h"
#include "Component.h"
#include "GameObject.h"

Shape::Shape(GameObject* gameObject, int vertexCount)
	:
    Component(gameObject),
	hr(0),
    texturePath(""),
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

std::string Shape::GetTexturePath() {
    return texturePath;
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

void Shape::SetTexturePath(std::string texturePath) {
    this->texturePath = std::string(texturePath);
}

void Shape::SetFaceColors(FaceColor* pFaceColors) {
    this->faceColors = pFaceColors;
}

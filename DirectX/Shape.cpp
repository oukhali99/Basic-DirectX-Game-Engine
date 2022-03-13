#include "Shape.h"
#include "Graphics.h"
#include "btBulletDynamicsCommon.h"
#include "Component.h"
#include "GameObject.h"
#include "Texture.h"

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

void Shape::SetTexture(Texture* texture) {
    this->texture = texture;
}

void Shape::SetFaceColors(FaceColor* pFaceColors) {
    this->faceColors = pFaceColors;
}

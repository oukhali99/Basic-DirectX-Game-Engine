#include "Shape.h"
#include "Graphics.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "Component.h"
#include "GameObject.h"
#include "Texture.h"
#include "Window.h"
#include "Game.h"

Shape::Shape(GameObject* gameObject, int vertexCount, int indexCount)
	:
    Component(gameObject),
	hr(0),
    texture(0),
    faceColors(0),
    vertexCount(vertexCount),
    indexCount(indexCount)
{
    vertices = (VERTEX*)malloc(vertexCount * sizeof(VERTEX));
    indices = (unsigned short*)malloc(indexCount * sizeof(unsigned short));
}

Shape::~Shape() {
    free(vertices);
    free(indices);
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

unsigned short* Shape::GetIndices() {
    return indices;
}

void Shape::SetTexture(Texture* texture) {
    this->texture = texture;
}

void Shape::SetFaceColors(FaceColor* pFaceColors) {
    this->faceColors = pFaceColors;
}

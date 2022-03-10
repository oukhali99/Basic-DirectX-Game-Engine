#include "Shape.h"
#include "Graphics.h"
#include "btBulletDynamicsCommon.h"

Shape::Shape()
	:
	hr(0),
    collisionObject(0)
{}

void Shape::RenderFrame() {
    for (Bindable* bindable : bindables) {
        bindable->Bind(this);
    }
}

btTransform Shape::GetTransform() {
    return transform;
}

btVector3 Shape::GetSize() {
    return size;
}

void Shape::SetSize(btVector3 size) {
    this->size = size;
}

void Shape::SetTransform(btTransform transform) {
    this->transform = transform;
}

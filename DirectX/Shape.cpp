#include "Shape.h"
#include "Graphics.h"
#include "btBulletDynamicsCommon.h"

Shape::Shape()
	:
	hr(0),
    collisionObject(0)
{}

void Shape::RenderFrame() {
    if (collisionObject) {
        btRigidBody* rigidbody = btRigidBody::upcast(collisionObject);
        if (rigidbody) {
            rigidbody->getMotionState()->getWorldTransform(transform);
        }
    }

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

btRigidBody* Shape::GetRigidbody() {
    if (collisionObject) {
        btRigidBody* rb = btRigidBody::upcast(collisionObject);
        if (rb) {
            return rb;
        }
    }

    return NULL;
}

#include "Shape.h"
#include "Graphics.h"
#include "btBulletDynamicsCommon.h"

Shape::Shape()
	:
	hr(0),
    followKeyboard(false),
    followMouse(false),
    collisionObject(0)
{
    // Initialize the transform
    transform.setIdentity();
    transform.setOrigin(btVector3(0, 0, 10));
}

void Shape::RenderFrame() {
    if (collisionObject) {
        btRigidBody* rigidbody = btRigidBody::upcast(collisionObject);
        if (rigidbody) {
            rigidbody->getMotionState()->getWorldTransform(transform);
        }
    }

    for (Bindable* bindable : bindables) {
        bindable->Bind(transform);
    }
}

void Shape::MouseMovedTo(Mouse::Position position) {
    if (followMouse) {
        OnMouseMovedTo(position);
    }
}

void Shape::ButtonPressed(WPARAM wParam) {
    if (followKeyboard) {
        OnButtonPressed(wParam);
    }
}

void Shape::SetTransform(btTransform transform) {
    this->transform = transform;
}

void Shape::OnButtonPressed(WPARAM wParam) {
    if (collisionObject) {
        btRigidBody* rigidbody = btRigidBody::upcast(collisionObject);
        if (rigidbody) {
            float increment = 0.1f;
            float force = 1.5f;
            btVector3 direction(0, 0, 0);

            if (wParam == 'w') {
                direction.setY(1);
            }
            else if (wParam == 's') {
                direction.setY(-1);
            }
            else if (wParam == 'd') {
                direction.setX(1);
            }
            else if (wParam == 'a') {
                direction.setX(-1);
            }

            rigidbody->activate();
            rigidbody->applyCentralImpulse(direction * force);
        }
    }
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

void Shape::OnMouseMovedTo(Mouse::Position position) { 
    /*
    transform.x = 6 * position.x;
    transform.y = 6 * position.y;
    */
}

ID3D11Device* Shape::GetDevice(Graphics* gfx) {
    return gfx->pDevice;
}

ID3D11DeviceContext* Shape::GetDeviceContext(Graphics* gfx) {
    return gfx->pContext;
}

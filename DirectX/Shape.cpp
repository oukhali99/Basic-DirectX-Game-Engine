#include "Shape.h"
#include "Graphics.h"

Shape::Shape(Graphics* gfx, btDiscreteDynamicsWorld* dynamicsWorld)
	:
	gfx(gfx),
	hr(0),
    followKeyboard(false),
    followMouse(false),
    dynamicsWorld(dynamicsWorld),
    collisionObject(0)
{
    // Initialize the transform
    transform.setIdentity();

    gfx->AddShape(this);
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

void Shape::AddRigidBody() {
    btCollisionShape* shape = new btBoxShape(btVector3(btScalar(1), btScalar(1), btScalar(1)));
    btScalar mass(1);
    bool isDynamic = (mass != 0);
    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        shape->calculateLocalInertia(mass, localInertia);
    btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
    collisionObject = new btRigidBody(rbInfo);
    dynamicsWorld->addRigidBody(btRigidBody::upcast(collisionObject));
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

void Shape::SetMass(btScalar mass) {
    btRigidBody* rb = GetRigidbody();
    if (rb) {
        btVector3 inertia;
        rb->getCollisionShape()->calculateLocalInertia(mass, inertia);
        rb->setMassProps(mass, inertia);

        dynamicsWorld->removeCollisionObject(rb);
        dynamicsWorld->addCollisionObject(rb);
    }
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

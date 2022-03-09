#include "Shape.h"
#include "Graphics.h"

Shape::Shape(Graphics* gfx, btDiscreteDynamicsWorld* dynamicsWorld)
	:
	gfx(gfx),
	hr(0),
    transform(transform),
    followKeyboard(false),
    followMouse(false),
    dynamicsWorld(dynamicsWorld)
{
    // Initialize the transform
    transform.setIdentity();

    gfx->AddShape(this);
}

void Shape::RenderFrame() {
    rigidbody->getMotionState()->getWorldTransform(transform);

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

void Shape::OnButtonPressed(WPARAM wParam) {
    float increment = 0.1f;
    float force = 0.5f;
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

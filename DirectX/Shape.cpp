#include "Shape.h"
#include "Graphics.h"

Shape::Shape(Graphics& gfx, btDiscreteDynamicsWorld* dynamicsWorld, btTransform transform)
	:
	gfx(gfx),
	hr(0),
    transform(transform),
    followKeyboard(false),
    followMouse(false),
    dynamicsWorld(dynamicsWorld)
{
    gfx.AddShape(this);
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

    /*
    if (wParam == 'w') {
        transform.y += increment;
    }
    else if (wParam == 's') {
        transform.y -= increment;
    }
    else if (wParam == 'd') {
        transform.x += increment;
    }
    else if (wParam == 'a') {
        transform.x -= increment;
    }
    */
}

void Shape::OnMouseMovedTo(Mouse::Position position) { 
    /*
    transform.x = 6 * position.x;
    transform.y = 6 * position.y;
    */
}

ID3D11Device* Shape::GetDevice(Graphics& gfx) {
    return gfx.pDevice;
}

ID3D11DeviceContext* Shape::GetDeviceContext(Graphics& gfx) {
    return gfx.pContext;
}

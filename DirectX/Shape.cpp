#include "Shape.h"

Shape::Shape(ID3D11Device& pDevice, ID3D11DeviceContext& pContext, Transform transform)
	:
	pDevice(pDevice),
	pContext(pContext),
	hr(0),
    transform(transform),
    followKeyboard(false),
    followMouse(false)
{
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
}

void Shape::OnMouseMovedTo(Mouse::Position position) {
    transform.x = 6 * position.x;
    transform.y = 6 * position.y;
}

#include "Shape.h"

Shape::Shape(ID3D11Device& pDevice, ID3D11DeviceContext& pContext, Transform transform)
	:
	pDevice(pDevice),
	pContext(pContext),
	hr(0),
    transform(transform),
    followMouse(false)
{}

Shape::~Shape() {
}

void Shape::MouseMovedTo(Mouse::Position position) {
    if (followMouse) {
        OnMouseMovedTo(position);
    }
}

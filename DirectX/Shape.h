#ifndef SHAPE_H
#define SHAPE_H

#include "Main.h"
#include "Mouse.h"
#include "Bindable.h"

class Shape {
public:
	bool followMouse;

	virtual void RenderFrame() = 0;
	virtual void OnButtonPressed(WPARAM wParam) = 0;
	virtual void OnMouseMovedTo(Mouse::Position position) = 0;

	void MouseMovedTo(Mouse::Position position);
protected:
	Shape(ID3D11Device& pDevice, ID3D11DeviceContext& pContext, Transform transform);
	~Shape();

	HRESULT hr;
	ID3D11Device& pDevice;
	ID3D11DeviceContext& pContext;
	Transform transform;
	std::vector<Bindable*> bindables;
};

#endif


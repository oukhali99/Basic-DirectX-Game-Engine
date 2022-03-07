#ifndef SHAPE_H
#define SHAPE_H

#include "Main.h"
#include "Mouse.h"
#include "Bindable.h"

class Shape {
public:
	bool followMouse;
	bool followKeyboard;

	virtual void RenderFrame() = 0;

	void ButtonPressed(WPARAM wParam);
	void MouseMovedTo(Mouse::Position position);
protected:
	Shape(ID3D11Device& pDevice, ID3D11DeviceContext& pContext, Transform transform);

	HRESULT hr;
	ID3D11Device& pDevice;
	ID3D11DeviceContext& pContext;
	Transform transform;
	std::vector<Bindable*> bindables;

	void OnButtonPressed(WPARAM wParam);
	void OnMouseMovedTo(Mouse::Position position);
};

#endif


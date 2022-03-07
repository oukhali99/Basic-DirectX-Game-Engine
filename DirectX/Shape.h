#ifndef SHAPE_H
#define SHAPE_H

#include "Main.h"
#include "Mouse.h"
#include "Bindable.h"

class Graphics;

class Shape {
public:
	bool followMouse;
	bool followKeyboard;

	virtual void RenderFrame() = 0;

	void ButtonPressed(WPARAM wParam);
	void MouseMovedTo(Mouse::Position position);
protected:
	Shape(Graphics& gfx, Transform transform);

	HRESULT hr;
	Graphics& gfx;
	Transform transform;
	std::vector<Bindable*> bindables;

	void OnButtonPressed(WPARAM wParam);
	void OnMouseMovedTo(Mouse::Position position);

	static ID3D11Device* GetDevice(Graphics& gfx);
	static ID3D11DeviceContext* GetDeviceContext(Graphics& gfx);
};

#endif


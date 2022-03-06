#ifndef SHAPE_H
#define SHAPE_H

#include "Main.h"
#include "Mouse.h"

class Shape {
public:
	struct Transform {
		float x, y, z;
		float xRot, yRot, zRot;
	};

	struct VERTEX {
		float x, y, z;
	};

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

	void HandleError(HRESULT hr, const char* file, const long long line);
};

#endif


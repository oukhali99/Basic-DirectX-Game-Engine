#ifndef SHAPE_H
#define SHAPE_H

#include "Main.h"
#include "Mouse.h"
#include "Bindable.h"
#include "btBulletDynamicsCommon.h"

class Graphics;

class Shape {
public:
	bool followMouse;
	bool followKeyboard;

	btRigidBody* GetRigidbody();

	void RenderFrame();
	void ButtonPressed(WPARAM wParam);
	void MouseMovedTo(Mouse::Position position);
	void SetTransform(btTransform transform);
protected:
	Shape();

	HRESULT hr;
	btTransform transform;
	std::vector<Bindable*> bindables;
	btCollisionObject* collisionObject;

	void OnButtonPressed(WPARAM wParam);
	void OnMouseMovedTo(Mouse::Position position);

	static ID3D11Device* GetDevice(Graphics* gfx);
	static ID3D11DeviceContext* GetDeviceContext(Graphics* gfx);
};

#endif


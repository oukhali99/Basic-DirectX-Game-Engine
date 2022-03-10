#ifndef SHAPE_H
#define SHAPE_H

#include "Main.h"
#include "Mouse.h"
#include "Bindable.h"
#include "btBulletDynamicsCommon.h"

class Graphics;

class Shape {
public:
	btRigidBody* GetRigidbody();

	btTransform GetTransform();
	btVector3 GetSize();

	void RenderFrame();
	void SetTransform(btTransform transform);
	void SetSize(btVector3 size);
protected:
	Shape();

	HRESULT hr;
	std::vector<Bindable*> bindables;

	btTransform transform;
	btVector3 size;
	btCollisionObject* collisionObject;
};

#endif


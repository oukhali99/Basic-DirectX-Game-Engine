#ifndef SHAPE_H
#define SHAPE_H

#include "Main.h"
#include "Mouse.h"
#include "Bindable.h"
#include "btBulletDynamicsCommon.h"

class Graphics;

class Shape {
public:
	btTransform GetTransform();
	btVector3 GetSize();

	virtual void RenderFrame() = 0;
	void SetTransform(btTransform transform);
	void SetSize(btVector3 size);
protected:
	Shape();

	HRESULT hr;
	btTransform transform;
	btVector3 size;
	btCollisionObject* collisionObject;
};

#endif


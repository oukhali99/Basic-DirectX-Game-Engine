#ifndef SHAPE_H
#define SHAPE_H

#include "Main.h"
#include "Mouse.h"
#include "Bindable.h"
#include "btBulletDynamicsCommon.h"
#include "Component.h"

class Graphics;
class Shape;

class Shape : public Component {
public:
	btTransform GetTransform();
	btVector3 GetScale();
protected:
	Shape(GameObject* gameObject);

	HRESULT hr;
};

#endif


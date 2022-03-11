#ifndef SHAPE_H
#define SHAPE_H

#include <string>
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
	std::string GetTexturePath();

	void SetTexturePath(std::string texturePath);
protected:
	Shape(GameObject* gameObject);

	HRESULT hr;
	std::string texturePath;
};

#endif


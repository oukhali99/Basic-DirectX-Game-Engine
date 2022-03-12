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
struct FaceColors;

class Shape : public Component {
public:
	btTransform GetTransform();
	btVector3 GetScale();
	std::string GetTexturePath();
	FaceColors* GetFaceColors();

	void SetTexturePath(std::string texturePath);
	void SetFaceColors(FaceColors* pFaceColors);
protected:
	Shape(GameObject* gameObject);

	HRESULT hr;
	std::string texturePath;
	FaceColors* faceColors;
};

#endif


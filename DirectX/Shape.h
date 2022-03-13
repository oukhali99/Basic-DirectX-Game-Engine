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
struct FaceColor;
struct VERTEX;

class Shape : public Component {
public:
	btTransform GetTransform();
	btVector3 GetScale();
	std::string GetTexturePath();
	FaceColor* GetFaceColors();
	VERTEX* GetVertices();
	int GetVertexCount();

	void SetTexturePath(std::string texturePath);
	void SetFaceColors(FaceColor* pFaceColors);
protected:
	Shape(GameObject* gameObject, int vertexCount);
	~Shape();

	HRESULT hr;
	std::string texturePath;
	FaceColor* faceColors;
	VERTEX* vertices;
	int vertexCount;
};

#endif


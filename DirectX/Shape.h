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
class Texture;
struct FaceColor;
struct VERTEX;

class Shape : public Component {
public:
	btTransform GetTransform();
	btVector3 GetScale();
	Texture* GetTexture();
	FaceColor* GetFaceColors();
	VERTEX* GetVertices();
	int GetVertexCount();

	void SetTexture(Texture* texture);
	void SetFaceColors(FaceColor* pFaceColors);
protected:
	Shape(GameObject* gameObject, int vertexCount);
	~Shape();

	HRESULT hr;
	Texture* texture;
	FaceColor* faceColors;
	VERTEX* vertices;
	int vertexCount;

	virtual void SetupVertices() = 0;
};

#endif


#ifndef SHAPE_H
#define SHAPE_H

#include <string>
#include "Main.h"
#include "Mouse.h"
#include "Bindable.h"
#include <bullet/btBulletDynamicsCommon.h>
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
	unsigned short* GetIndices();

	void SetTexture(Texture* texture);
	void SetFaceColors(FaceColor* pFaceColors);
protected:
	Shape(GameObject* gameObject, int vertexCount, int indexCount);
	~Shape();

	HRESULT hr;
	Texture* texture;
	FaceColor* faceColors;
	VERTEX* vertices;
	int vertexCount;
	unsigned short* indices;
	int indexCount;

	virtual void SetupVertices() = 0;
	virtual void SetupIndices() = 0;
};

#endif


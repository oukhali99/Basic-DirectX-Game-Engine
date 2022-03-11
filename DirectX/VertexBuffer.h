#include "Main.h"
#include "Bindable.h"
#include "btBulletDynamicsCommon.h"
#include "Graphics.h"

class Shape;

class VertexBuffer : public Bindable {
public:
	VertexBuffer(VERTEX* vertices, int verticesSize);
	void Bind(Shape* shape) override;
};

#include "Main.h"
#include "Bindable.h"
#include <bullet/btBulletDynamicsCommon.h>
#include "Graphics.h"

class Shape;

class VertexBuffer : public Bindable {
public:
	VertexBuffer(int vertexCount);
	void Bind(Shape* shape) override;
};

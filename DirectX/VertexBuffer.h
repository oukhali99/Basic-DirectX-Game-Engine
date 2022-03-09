#include "Main.h"
#include "Bindable.h"
#include "btBulletDynamicsCommon.h"

class VertexBuffer : public Bindable {
public:
	VertexBuffer(Graphics* gfx, VERTEX* vertices, int verticesSize);
	void Bind(btTransform transform) override;
};

#include "Main.h"
#include "Bindable.h"
#include "btBulletDynamicsCommon.h"

class VertexBuffer : public Bindable {
public:
	VertexBuffer(VERTEX* vertices, int verticesSize);
	void Bind(btTransform transform) override;
};

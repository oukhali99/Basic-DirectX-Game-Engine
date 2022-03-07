#include "Main.h"
#include "Bindable.h"

class VertexBuffer : public Bindable {
public:
	VertexBuffer(Graphics& gfx, VERTEX* vertices, int verticesSize);
	void Bind(Transform transform) override;
};

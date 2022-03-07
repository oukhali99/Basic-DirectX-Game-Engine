#include "Main.h"
#include "Bindable.h"
#include "Shape.h"

class VertexBuffer : public Bindable {
public:
	VertexBuffer(ID3D11DeviceContext* pContext, ID3D11Device* pDevice, VERTEX* vertices, int verticesSize);
	void Bind(Transform transform) override;
};

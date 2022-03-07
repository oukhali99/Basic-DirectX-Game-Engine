#include "Main.h"
#include "Bindable.h"

class IndexBuffer : public Bindable {
public:
	IndexBuffer(ID3D11DeviceContext* pContext, ID3D11Device* pDevice, unsigned short indices[], UINT sizeOfIndices);
	void Bind(Transform transform) override;
};

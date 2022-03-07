#include "Bindable.h"
#include "Shape.h"

class TransformConstantBuffer : public Bindable {
public:
	TransformConstantBuffer(ID3D11DeviceContext* pContext, ID3D11Device* pDevice);
	void Bind(Transform transform) override;
private:
	ConstantBuffer constantBuffer;
};

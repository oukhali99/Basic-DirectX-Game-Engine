#include "Main.h"
#include "Bindable.h"

class ColorConstantBuffer : public Bindable {
public:
	ColorConstantBuffer(ID3D11DeviceContext* pContext, ID3D11Device* pDevice, FaceColors fc);
	void Bind(Transform transform) override;
};

#ifndef H_BINDABLE
#define H_BINDABLE
#include "Main.h"

class Bindable {
public:
	Bindable(ID3D11DeviceContext* pContext, ID3D11Device* pDevice);
	virtual void Bind(Transform transform) = 0;
protected:
	HRESULT hr;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;
	ID3D11Buffer* pBuffer;
};
#endif

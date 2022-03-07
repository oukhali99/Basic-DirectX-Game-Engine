#include "Bindable.h"

Bindable::Bindable(ID3D11DeviceContext* pContext, ID3D11Device* pDevice)
	:
	pDevice(pDevice),
	pContext(pContext)
{}

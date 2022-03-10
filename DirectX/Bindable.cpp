#include "Bindable.h"
#include "Graphics.h"

Bindable::Bindable(Graphics* gfx)
	:
	gfx(gfx)
{}

ID3D11Device* Bindable::GetDevice(Graphics* gfx) {
	return gfx->pDevice;
}

ID3D11DeviceContext* Bindable::GetDeviceContext(Graphics* gfx) {
	return gfx->pContext;
}

float Bindable::GetNearZ(Graphics* gfx) {
	return gfx->nearZ;
}

float Bindable::GetFarZ(Graphics* gfx) {
	return gfx->farZ;
}

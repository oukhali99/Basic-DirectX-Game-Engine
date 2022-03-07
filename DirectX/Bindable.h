#ifndef H_BINDABLE
#define H_BINDABLE
#include "Main.h"

class Graphics;

class Bindable {
public:
	Bindable(Bindable& bindable) = delete;

	Bindable(Graphics& gfx);

	virtual void Bind(Transform transform) = 0;

	static ID3D11Device* GetDevice(Graphics& gfx);
	static ID3D11DeviceContext* GetDeviceContext(Graphics& gfx);
protected:
	HRESULT hr;
	ID3D11Buffer* pBuffer;
	Graphics& gfx;
};
#endif

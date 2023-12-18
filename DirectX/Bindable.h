#ifndef H_BINDABLE
#define H_BINDABLE
#include "Main.h"
#include <bullet/btBulletDynamicsCommon.h>

class Graphics;
class Shape;

class Bindable {
public:
	Bindable(Bindable& bindable) = delete;

	Bindable();

	virtual void Bind(Shape* shape) = 0;
protected:
	HRESULT hr;
	ID3D11Buffer* pBuffer;
};
#endif

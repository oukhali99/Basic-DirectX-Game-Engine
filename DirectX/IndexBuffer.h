#include "Main.h"
#include "Bindable.h"
#include "btBulletDynamicsCommon.h"

class Shape;

class IndexBuffer : public Bindable {
public:
	IndexBuffer(unsigned short* indices, int indexCount);
	void Bind(Shape* shape) override;
private:
	int indexCount;
};

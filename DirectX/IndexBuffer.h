#include "Main.h"
#include "Bindable.h"
#include "btBulletDynamicsCommon.h"

class Shape;

class IndexBuffer : public Bindable {
public:
	IndexBuffer(unsigned short indices[], UINT sizeOfIndices);
	void Bind(Shape* shape) override;
private:
	unsigned short* indices; 
	UINT sizeOfIndices;
};

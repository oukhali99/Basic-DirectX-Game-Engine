#include "Main.h"
#include "Bindable.h"
#include "btBulletDynamicsCommon.h"

class IndexBuffer : public Bindable {
public:
	IndexBuffer(unsigned short indices[], UINT sizeOfIndices);
	void Bind(btTransform transform) override;
private:
	unsigned short* indices; 
	UINT sizeOfIndices;
};

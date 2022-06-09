#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

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

#endif

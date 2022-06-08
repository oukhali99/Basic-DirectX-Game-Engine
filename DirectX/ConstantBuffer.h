#ifndef CONSTANT_BUFFER_H
#define CONSTANT_BUFFER_H
#include "Bindable.h"

class ConstantBuffer : public Bindable {
public:
	void Bind(Shape* shape) override;
protected:
	ConstantBuffer(size_t bufferSize);
	virtual UINT GetSlotNumber() = 0;
	virtual size_t GetBufferSize() = 0;
	virtual const void* GetBufferData(Shape* shape) = 0;
};
#endif

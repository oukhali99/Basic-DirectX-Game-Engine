#ifndef COLOR_CONSTANT_BUFFER_H
#define COLOR_CONSTANT_BUFFER_H
#include "ConstantBuffer.h"

class ColorConstantBuffer : public ConstantBuffer {
public:
	ColorConstantBuffer(int faceCount);

	struct Data {
		float r, g, b, a;
	};
protected:
	UINT GetSlotNumber();
	size_t GetBufferSize();
	const void* GetBufferData(Shape* shape);
private:
	int faceCount;
};
#endif

#include "ColorConstantBuffer.h"
#include "Shape.h"

ColorConstantBuffer::ColorConstantBuffer(int faceCount)
	:
	ConstantBuffer(sizeof(Data) * faceCount),
	faceCount(faceCount)
{}

UINT ColorConstantBuffer::GetSlotNumber() {
	return 1u;
}

size_t ColorConstantBuffer::GetBufferSize() {
	return sizeof(Data) * faceCount;
}

const void* ColorConstantBuffer::GetBufferData(Shape* shape) {
	return shape->GetFaceColors();
}

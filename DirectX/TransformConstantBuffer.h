#include "ConstantBuffer.h"

class TransformConstantBuffer : public ConstantBuffer {
public:
	TransformConstantBuffer();

	struct Data {
		dx::XMMATRIX worldTransformation;
		dx::XMMATRIX viewTransformation;
	};
protected:
	UINT GetSlotNumber() override;
	size_t GetBufferSize() override;
	const void* GetBufferData(Shape* shape) override;
};

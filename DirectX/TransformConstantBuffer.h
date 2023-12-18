#include "ConstantBuffer.h"

class TransformConstantBuffer : public ConstantBuffer {
public:
	TransformConstantBuffer();
	~TransformConstantBuffer();

	struct Data {
		dx::XMMATRIX worldTransformation;
		dx::XMMATRIX viewTransformation;
		dx::XMMATRIX projectionTransformation;
	};
protected:
	UINT GetSlotNumber() override;
	size_t GetBufferSize() override;
	const void* GetBufferData(Shape* shape) override;

	Data* data;
};

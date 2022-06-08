#include "Bindable.h"

class ConstantBuffer : public Bindable {
public:
	ConstantBuffer(UINT slotNumber, size_t size);

	void Bind(Shape* shape) override;
private:
	UINT slotNumber;
	size_t size;
};

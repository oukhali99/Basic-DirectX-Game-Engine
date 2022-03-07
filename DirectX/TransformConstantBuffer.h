#include "Bindable.h"
#include "Shape.h"

class TransformConstantBuffer : public Bindable {
public:
	TransformConstantBuffer(Graphics& gfx);
	void Bind(Transform transform) override;
private:
	ConstantBuffer constantBuffer;
};

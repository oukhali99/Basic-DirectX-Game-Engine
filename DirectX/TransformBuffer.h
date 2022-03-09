#include "Bindable.h"
#include "Shape.h"

class TransformBuffer : public Bindable {
public:
	TransformBuffer(Graphics& gfx);
	void Bind(btTransform transform) override;
};

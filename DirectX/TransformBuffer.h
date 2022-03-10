#include "Bindable.h"
#include "Shape.h"

class TransformBuffer : public Bindable {
public:
	TransformBuffer();
	void Bind(Shape* shape) override;
};

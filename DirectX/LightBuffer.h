#include "Bindable.h"

class LightBuffer : public Bindable {
public:
	LightBuffer();

	void Bind(Shape* shape) override;
};

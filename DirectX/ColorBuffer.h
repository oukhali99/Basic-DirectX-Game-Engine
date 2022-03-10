#include "Main.h"
#include "Bindable.h"
#include "Graphics.h"

class Shape;

class ColorBuffer : public Bindable {
public:
	ColorBuffer(FaceColors fc);
	void Bind(Shape* shape) override;
};

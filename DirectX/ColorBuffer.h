#include "Main.h"
#include "Bindable.h"
#include "Graphics.h"

class Shape;

class ColorBuffer : public Bindable {
public:
	ColorBuffer(unsigned short faceCount);
	void Bind(Shape* shape) override;
private:
	unsigned short faceCount;
};

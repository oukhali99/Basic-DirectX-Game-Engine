#include "Main.h"
#include "Shape.h"


class Pyramid : public Shape {
public:
	Pyramid(Graphics& gfx, Transform transform);
	void RenderFrame() override;
};

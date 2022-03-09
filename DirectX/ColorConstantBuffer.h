#include "Main.h"
#include "Bindable.h"
#include "Graphics.h"

class ColorConstantBuffer : public Bindable {
public:
	ColorConstantBuffer(Graphics& gfx, FaceColors fc);
	void Bind(btTransform transform) override;
};

#include "Main.h"
#include "Bindable.h"
#include "Graphics.h"

class ColorBuffer : public Bindable {
public:
	ColorBuffer(FaceColors fc);
	void Bind(btTransform transform) override;
};

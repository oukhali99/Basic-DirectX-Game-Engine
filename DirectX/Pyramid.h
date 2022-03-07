#include "Main.h"
#include "Shape.h"


class Pyramid : public Shape {
public:
	Pyramid(ID3D11Device& pDevice, ID3D11DeviceContext& pContext, Transform transform);
	void RenderFrame() override;
};

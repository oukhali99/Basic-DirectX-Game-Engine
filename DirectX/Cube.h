#include "Main.h"
#include "Shape.h"
#include "Clock.h"
#include "Mouse.h"
#include "VertexBuffer.h"
#include "TransformConstantBuffer.h"
#include "ColorConstantBuffer.h"
#include "IndexBuffer.h"

class Cube : public Shape {
public:
	Cube(ID3D11Device& pDevice, ID3D11DeviceContext& pContext, Transform transform);
    void RenderFrame() override;
};

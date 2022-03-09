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
	Cube(Graphics& gfx, btDiscreteDynamicsWorld* dynamicsWorld, btTransform transform);
};

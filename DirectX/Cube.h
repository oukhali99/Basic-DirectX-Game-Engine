#include "Main.h"
#include "ShapeBase.h"
#include "Clock.h"
#include "Mouse.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Cube : public ShapeBase<Cube> {
public:
	Cube(GameObject* gameObject);
};

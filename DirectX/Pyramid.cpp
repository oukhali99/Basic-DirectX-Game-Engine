#include "Pyramid.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Clock.h"
#include "Game.h"
#include "ConstantBuffer.h"

Pyramid::Pyramid(GameObject* gameObject)
	:
	ShapeBase<Pyramid>(gameObject, 4)
{
	VERTEX vertices[] = {
		{ 0.0f, 0.0f, -1.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.5f, -0.5f, 0.0f},
		{ -0.5f, -0.5f, 0.0f }
	};

	unsigned short indices[] = {
		0, 1, 2,	// Right face
		0, 2, 3,	// Bottom face
		0, 3, 1,	// Left face
		1, 3, 2		// Floor
	};

	if (bindables.size() == 0) {
		VertexBuffer* vb = new VertexBuffer(vertexCount);
		bindables.push_back(vb);

		ConstantBuffer* tcb = new ConstantBuffer(0u, sizeof(ConstantBuffer::Transform));
		bindables.push_back(tcb);

		ConstantBuffer* ccb = new ConstantBuffer(1u, sizeof(FaceColor));
		bindables.push_back(ccb);

		IndexBuffer* ib = new IndexBuffer(indices, sizeof(indices));
		bindables.push_back(ib);
	}
}

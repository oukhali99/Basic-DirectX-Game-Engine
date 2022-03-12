#include "Pyramid.h"
#include "VertexBuffer.h"
#include "ColorBuffer.h"
#include "IndexBuffer.h"
#include "TransformBuffer.h"
#include "Clock.h"
#include "Game.h"

Pyramid::Pyramid(GameObject* gameObject)
	:
	ShapeBase<Pyramid>(gameObject)
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
		VertexBuffer* vb = new VertexBuffer(vertices, sizeof(vertices));
		bindables.push_back(vb);

		TransformBuffer* tcb = new TransformBuffer();
		bindables.push_back(tcb);

		ColorBuffer* ccb = new ColorBuffer(4);
		bindables.push_back(ccb);

		IndexBuffer* ib = new IndexBuffer(indices, sizeof(indices));
		bindables.push_back(ib);
	}
}

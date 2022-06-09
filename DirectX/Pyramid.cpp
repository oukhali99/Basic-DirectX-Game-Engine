#include "Pyramid.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Clock.h"
#include "Game.h"
#include "TransformConstantBuffer.h"
#include "ColorConstantBuffer.h"

Pyramid::Pyramid(GameObject* gameObject)
	:
	ShapeBase<Pyramid>(gameObject, 4, 12)
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

		TransformConstantBuffer* tcb = new TransformConstantBuffer();
		bindables.push_back(tcb);

		ColorConstantBuffer* ccb = new ColorConstantBuffer(4);
		bindables.push_back(ccb);

		IndexBuffer* ib = new IndexBuffer(indices, indexCount);
		bindables.push_back(ib);
	}
}

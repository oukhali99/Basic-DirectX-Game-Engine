#include "Pyramid.h"
#include "VertexBuffer.h"
#include "ColorBuffer.h"
#include "IndexBuffer.h"
#include "TransformBuffer.h"
#include "Clock.h"

Pyramid::Pyramid(Graphics* gfx, btDiscreteDynamicsWorld* dynamicsWorld)
	:
	Shape(gfx, dynamicsWorld)
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

	FaceColors fc = {
		{
			{ 1, 0, 0, 1 },
			{ 0, 1, 0, 1 },
			{ 0, 0, 1, 1 },
			{ 0, 0, 0, 1 }
		}
	};

	VertexBuffer* vb = new VertexBuffer(gfx, vertices, sizeof(vertices));
	bindables.push_back(vb);

	TransformBuffer* tcb = new TransformBuffer(gfx);
	bindables.push_back(tcb);

	ColorBuffer* ccb = new ColorBuffer(gfx, fc);
	bindables.push_back(ccb);

	IndexBuffer* ib = new IndexBuffer(gfx, indices, sizeof(indices));
	bindables.push_back(ib);
}

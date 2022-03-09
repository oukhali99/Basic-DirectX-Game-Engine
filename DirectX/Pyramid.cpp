#include "Pyramid.h"
#include "VertexBuffer.h"
#include "ColorConstantBuffer.h"
#include "IndexBuffer.h"
#include "TransformConstantBuffer.h"
#include "Clock.h"

Pyramid::Pyramid(Graphics& gfx, btDiscreteDynamicsWorld* dynamicsWorld, btTransform transform)
	:
	Shape(gfx, dynamicsWorld, transform)
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

	TransformConstantBuffer* tcb = new TransformConstantBuffer(gfx);
	bindables.push_back(tcb);

	ColorConstantBuffer* ccb = new ColorConstantBuffer(gfx, fc);
	bindables.push_back(ccb);

	IndexBuffer* ib = new IndexBuffer(gfx, indices, sizeof(indices));
	bindables.push_back(ib);
}

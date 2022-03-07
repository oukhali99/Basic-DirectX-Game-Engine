#include "Pyramid.h"
#include "VertexBuffer.h"
#include "ColorConstantBuffer.h"
#include "IndexBuffer.h"
#include "TransformConstantBuffer.h"
#include "Clock.h"

Pyramid::Pyramid(ID3D11Device& pDevice, ID3D11DeviceContext& pContext, Transform transform)
	:
	Shape(pDevice, pContext, transform)
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

	VertexBuffer* vb = new VertexBuffer(&pContext, &pDevice, vertices, sizeof(vertices));
	bindables.push_back(vb);

	TransformConstantBuffer* tcb = new TransformConstantBuffer(&pContext, &pDevice);
	bindables.push_back(tcb);

	ColorConstantBuffer* ccb = new ColorConstantBuffer(&pContext, &pDevice, fc);
	bindables.push_back(ccb);

	IndexBuffer* ib = new IndexBuffer(&pContext, &pDevice, indices, sizeof(indices));
	bindables.push_back(ib);
}

void Pyramid::RenderFrame() {
	// Get the time
	float t = Clock::GetSingleton().GetTimeSinceStart();
	//transform.xRot = 2 * t;
	//transform.yRot = 3 * t;
	//transform.zRot = 1 * t;

	for (Bindable* bindable : bindables) {
		bindable->Bind(transform);
	}

	// draw the vertex buffer to the back buffer
	pContext.DrawIndexed(12u, 0u, 0u);
}

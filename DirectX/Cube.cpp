#include "Cube.h"
#include "btBulletDynamicsCommon.h"
#include "Game.h"

Cube::Cube() {
    // Create a resource for the vertices
    VERTEX OurVertices[] = {
        {1.0f, 1.0f, -1.0f},

        {1.0f, -1.0f, -1.0f},

        {-1.0f, -1.0f, -1.0f},

        {-1.0f, 1.0f, -1.0f},

        {1.0f, 1.0f, 1.0f},

        {1.0f, -1.0f, 1.0f},

        {-1.0f, -1.0f, 1.0f},

        {-1.0f, 1.0f, 1.0f},
    };

    // Create the face color resource
    FaceColors fc = {
        {
            { 1.0f, 0.0f, 0.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f, 1.0f },
            { 0.0f, 0.0f, 1.0f, 1.0f },
            { 1.0f, 0.0f, 1.0f, 1.0f },
            { 1.0f, 1.0f, 0.0f, 1.0f },
        }
    };

    unsigned short indices[] = {
            0, 1, 2, 3, 0, 2,
            4, 6, 5, 6, 4, 7,
            0, 4, 1, 1, 4, 5,
            3, 6, 7, 3, 2, 6,
            3, 7, 0, 0, 7, 4,
            2, 5, 6, 1, 5, 2,
    };

    if (bindables.size() == 0) {
        VertexBuffer* vb = new VertexBuffer(OurVertices, sizeof(OurVertices));
        TransformBuffer* tcb = new TransformBuffer();
        ColorBuffer* ccb = new ColorBuffer(fc);
        IndexBuffer* ib = new IndexBuffer(indices, sizeof(indices));
        bindables.push_back(vb);
        bindables.push_back(tcb);
        bindables.push_back(ccb);
        bindables.push_back(ib);
    }
}

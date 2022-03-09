#include "Cube.h"
#include "btBulletDynamicsCommon.h"

Cube::Cube(Graphics& gfx, btDiscreteDynamicsWorld* dynamicsWorld, btTransform transform)
    :
    Shape(gfx, dynamicsWorld, transform)
{
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

    VertexBuffer* vb = new VertexBuffer(gfx, OurVertices, sizeof(OurVertices));
    bindables.push_back(vb);

    TransformBuffer* tcb = new TransformBuffer(gfx);
    bindables.push_back(tcb);

    ColorBuffer* ccb = new ColorBuffer(gfx, fc);
    bindables.push_back(ccb);

    IndexBuffer* ib = new IndexBuffer(gfx, indices, sizeof(indices));
    bindables.push_back(ib);

    btCollisionShape* shape = new btBoxShape(btVector3(btScalar(1), btScalar(1), btScalar(1)));
    btScalar mass(0.1f);
    bool isDynamic = (mass != 0.f);
    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        shape->calculateLocalInertia(mass, localInertia);
    btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
    rigidbody = new btRigidBody(rbInfo);
    dynamicsWorld->addRigidBody(rigidbody);
}

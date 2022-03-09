#include "Cube.h"
#include "btBulletDynamicsCommon.h"

Cube::Cube(Graphics* gfx, btDiscreteDynamicsWorld* dynamicsWorld)
    :
    Shape(gfx, dynamicsWorld)
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

    // TODO. find a better way
    transform.setOrigin(btVector3(0, 0, 10));

    VertexBuffer* vb = new VertexBuffer(gfx, OurVertices, sizeof(OurVertices));
    TransformBuffer* tcb = new TransformBuffer(gfx);
    ColorBuffer* ccb = new ColorBuffer(gfx, fc);
    IndexBuffer* ib = new IndexBuffer(gfx, indices, sizeof(indices));
    bindables.push_back(vb);
    bindables.push_back(tcb);
    bindables.push_back(ccb);
    bindables.push_back(ib);

    btCollisionShape* shape = new btBoxShape(btVector3(btScalar(1), btScalar(1), btScalar(1)));
    btScalar mass(1);
    bool isDynamic = (mass != 0);
    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        shape->calculateLocalInertia(mass, localInertia);
    btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
    collisionObject = new btRigidBody(rbInfo);
    dynamicsWorld->addRigidBody(btRigidBody::upcast(collisionObject));
}

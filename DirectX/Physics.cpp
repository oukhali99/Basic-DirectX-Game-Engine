#include "Physics.h"

Physics::Physics(btDiscreteDynamicsWorld** dynamicsWorld) {
    // Create the configuration
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    // Create the collision dispatcher
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    // ???
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
    // Constraint solver
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
    // Create the world
    this->dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
        overlappingPairCache, solver, collisionConfiguration);
    // Set the gravity
    this->dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

    // Set the return value
    *dynamicsWorld = this->dynamicsWorld;
}

Physics::~Physics() {
    delete this->dynamicsWorld;
}

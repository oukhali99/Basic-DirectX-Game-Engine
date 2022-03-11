#include "Physics.h"
#include "Clock.h"
#include "Game.h"
#include "GameObject.h"

void Physics::Init() {
    instance = new Physics();
}

Physics* Physics::GetInstance() {
    return instance;
}

Physics::Physics() {
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
}

void Physics::AddRigidbody(btRigidBody* rigidbody) {
    dynamicsWorld->addRigidBody(rigidbody);
}

void Physics::RemoveRigidbody(btRigidBody* rigidbody) {
    dynamicsWorld->removeRigidBody(rigidbody);
}

void Physics::Update() {
    static float last = Clock::GetSingleton().GetTimeSinceStart();
    this->dynamicsWorld->stepSimulation(Clock::GetSingleton().GetTimeSinceStart() - last, 10);
    last = Clock::GetSingleton().GetTimeSinceStart();
}

Physics::~Physics() {
    delete this->dynamicsWorld;
}

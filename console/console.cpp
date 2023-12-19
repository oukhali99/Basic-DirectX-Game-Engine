// console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <btBulletDynamicsCommon.h>

int main()
{
	// Create the configuration
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	// Create the collision dispatcher
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// ???
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	// Constraint solver
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
	
	// Create the world
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
		overlappingPairCache, solver, collisionConfiguration);

	// Set the gravity
	dynamicsWorld -> setGravity(btVector3(0, -10, 0));

	{
		// Create the ground
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -56, 0));
		btScalar mass(0.);
		bool isDynamic = (mass != 0.f);
		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		dynamicsWorld->addRigidBody(body);
	}

	{
		// Create the box
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(1), btScalar(1), btScalar(1)));

		btTransform groundTransform;
		groundTransform.setIdentity();
		//groundTransform.setOrigin(btVector3(0, 6, 0));

		btScalar mass(1);
		bool isDynamic = (mass != 0.f);
		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		dynamicsWorld->addRigidBody(body);
	}

	for (int i = 0; i < 100; i++) {
		dynamicsWorld->stepSimulation(1.0f / 60.0f, 10);

		for (int j = 0; j < dynamicsWorld->getNumCollisionObjects(); j++) {
			btCollisionObject* collisionObject = dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(collisionObject);
			btTransform trans;

			if (body && body->getMotionState()) {
				if (body->getMass() == 0) {
					continue;
				}
				body->getMotionState()->getWorldTransform(trans);

				std::cout << trans.getOrigin().getY() << std::endl;
			}
		}
	}

	return 0;
}
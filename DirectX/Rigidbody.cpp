#include "Rigidbody.h"
#include "btBulletDynamicsCommon.h"
#include "Component.h"
#include "GameObject.h"
#include "Physics.h"

Rigidbody::Rigidbody(GameObject* gameObject)
	:
	Component(gameObject)
{
	btScalar mass = 0;
	btCollisionShape* shape = new btBoxShape(gameObject->GetScale());
	bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);
	btDefaultMotionState* myMotionState = new btDefaultMotionState(gameObject->GetTransform());
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	rigidbody = new btRigidBody(rbInfo);
	Physics::GetInstance()->AddRigidbody(rigidbody);
}

void Rigidbody::Update() {
	btTransform transform;
	rigidbody->getMotionState()->getWorldTransform(transform);
	gameObject->SetTransform(transform);
}

void Rigidbody::SetMass(btScalar mass) {
	Physics::GetInstance()->RemoveRigidbody(rigidbody);
	btCollisionShape* shape = new btBoxShape(gameObject->GetScale());
	bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);
	btDefaultMotionState* myMotionState = new btDefaultMotionState(gameObject->GetTransform());
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	rigidbody = new btRigidBody(rbInfo);
	Physics::GetInstance()->AddRigidbody(rigidbody);
}

void Rigidbody::ApplyForce(btVector3 force) {
	rigidbody->applyCentralImpulse(force);
}

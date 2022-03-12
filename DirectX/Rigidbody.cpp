#include "Rigidbody.h"
#include "btBulletDynamicsCommon.h"
#include "Component.h"
#include "GameObject.h"
#include "Physics.h"

Rigidbody::Rigidbody(GameObject* gameObject)
	:
	Component(gameObject),
	isKinematic(false)
{
	btScalar mass = 0;
	btCollisionShape* shape = new btBoxShape(gameObject->GetScale());
	btVector3 localInertia(0, 0, 0);
	shape->calculateLocalInertia(mass, localInertia);
	btDefaultMotionState* myMotionState = new btDefaultMotionState(gameObject->GetTransform());
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	rigidbody = new btRigidBody(rbInfo);
	Physics::GetInstance()->AddRigidbody(rigidbody);
}

void Rigidbody::SetIsKinematic(bool isKinematic) {
	this->isKinematic = isKinematic;

	if (isKinematic) {
		rigidbody->setCollisionFlags(rigidbody->getCollisionFlags() |
			btCollisionObject::CF_KINEMATIC_OBJECT);
		rigidbody->setActivationState(DISABLE_DEACTIVATION);

		// Correction if necessary
		SetMass(0);
	}
}

void Rigidbody::Update() {
	btTransform transform;
	rigidbody->getMotionState()->getWorldTransform(transform);
	gameObject->SetTransform(transform);
}

void Rigidbody::SetMass(btScalar mass) {
	if (isKinematic) {
		return;
	}

	Physics::GetInstance()->RemoveRigidbody(rigidbody);
	btCollisionShape* shape = new btBoxShape(gameObject->GetScale());
	btVector3 localInertia(0, 0, 0);
	shape->calculateLocalInertia(mass, localInertia);
	btDefaultMotionState* myMotionState = new btDefaultMotionState(gameObject->GetTransform());
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	rigidbody = new btRigidBody(rbInfo);
	Physics::GetInstance()->AddRigidbody(rigidbody);
}

void Rigidbody::ApplyImpulse(btVector3 force) {
	if (rigidbody->getMass() > 0) {
		rigidbody->applyCentralImpulse(force);
	}
	else if (isKinematic) {
		btTransform newTrans;
		rigidbody->getMotionState()->getWorldTransform(newTrans);
		newTrans.getOrigin() += force;
		rigidbody->getMotionState()->setWorldTransform(newTrans);
	}
}

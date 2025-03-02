#include "Rigidbody.h"
#include "bullet/btBulletDynamicsCommon.h"
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

btVector3 Rigidbody::GetLinearVelocity() {
	return rigidbody->getLinearVelocity();
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
	if (isKinematic && mass > 0) {
		Main::HandleError(0, __FILE__, __LINE__, "Tried to set mass on Kinematic Object");
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
		rigidbody->activate();
		rigidbody->applyCentralImpulse(force);
	}
	else if (isKinematic) {
		btTransform newTrans;
		rigidbody->getMotionState()->getWorldTransform(newTrans);
		newTrans.getOrigin() += force;
		rigidbody->getMotionState()->setWorldTransform(newTrans);
	}
}

void Rigidbody::ApplyTorqueImpulse(btVector3 torque) {
	if (rigidbody->getMass() > 0) {
		rigidbody->applyTorqueImpulse(torque);
	}
	else if (isKinematic) {
		btTransform newTrans;
		rigidbody->getMotionState()->getWorldTransform(newTrans);
		newTrans.setRotation(btQuaternion(torque.getY(), torque.getX(), torque.getZ()) * newTrans.getRotation());
		rigidbody->getMotionState()->setWorldTransform(newTrans);
	}
}

void Rigidbody::SetGravity(btVector3 gravity) {
	rigidbody->setGravity(gravity);
}

void Rigidbody::SetFriction(btScalar friction) {
	rigidbody->setFriction(friction);
}

void Rigidbody::SetAngularFactor(btVector3 factor) {
	rigidbody->setAngularFactor(factor);
}

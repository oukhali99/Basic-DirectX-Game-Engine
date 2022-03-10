#include "GameObject.h"
#include "Game.h"
#include "btBulletDynamicsCommon.h"

GameObject::GameObject()
	:
	shape(0),
	rigidbody(0),
	transform(btTransform()),
	mass(0),
	size(btVector3(1, 1, 1))
{
	Game::GetInstance()->AddGameObject(this);
}

GameObject::GameObject(btTransform transform, btVector3 size, btScalar mass) 
	:
	shape(0),
	rigidbody(0),
	transform(transform),
	size(size),
	mass(mass)
{
	Game::GetInstance()->AddGameObject(this);
}

void GameObject::UpdatePhysics() {
	if (rigidbody) {
		rigidbody->getMotionState()->getWorldTransform(transform);

		if (shape) {
			shape->SetTransform(transform);
		}
	}
}

void GameObject::RenderFrame() {
	if (shape) {
		shape->RenderFrame();
	}
}

void GameObject::AddShape(Shape* shape) {
	shape->SetSize(size);
	this->shape = shape;
}

void GameObject::AddRigidbody() {
	btCollisionShape* shape = new btBoxShape(size);
	bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	rigidbody = new btRigidBody(rbInfo);
	Physics::GetInstance()->AddRigidbody(rigidbody);
}

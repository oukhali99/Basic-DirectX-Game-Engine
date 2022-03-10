#include "GameObject.h"
#include "Game.h"
#include "btBulletDynamicsCommon.h"

GameObject::GameObject()
	:
	shape(0),
	rigidbody(0)
{
	Game::GetInstance()->AddGameObject(this);
}

Shape* GameObject::GetShape() {
	return shape;
}

void GameObject::UpdatePhysics() {
	if (rigidbody) {
		btTransform newpos;
		rigidbody->getMotionState()->getWorldTransform(newpos);

		if (shape) {
			shape->SetTransform(newpos);
		}
	}
}

void GameObject::RenderFrame() {
	if (shape) {
		// Get the mouse position
		//Mouse::Position mousePosition = Mouse::GetSingleton(hWnd).GetPosition();

		//shape->MouseMovedTo(mousePosition);

		shape->RenderFrame();
	}
}

void GameObject::SetShape(Shape* shape) {
	this->shape = shape;
}

void GameObject::AddRigidbody() {
	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(1), btScalar(1), btScalar(1)));
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, 0, 10));
	btScalar mass(1);
	bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		groundShape->calculateLocalInertia(mass, localInertia);
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	rigidbody = new btRigidBody(rbInfo);
	Physics::GetInstance()->AddRigidbody(rigidbody);
}

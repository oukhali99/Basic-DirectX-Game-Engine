#ifndef H_GAMEOBJECT
#define H_GAMEOBJECT
#include "Shape.h"
#include "btBulletDynamicsCommon.h"

class GameObject {
public:
	GameObject();

	void RenderFrame();
	void UpdatePhysics();
	void AddShape(Shape* shape);
	void AddRigidbody();
private:
	Shape* shape;
	btRigidBody* rigidbody;
	btTransform transform;
	btVector3 size;
	btScalar mass;
};
#endif

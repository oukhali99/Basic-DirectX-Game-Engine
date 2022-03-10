#ifndef H_GAMEOBJECT
#define H_GAMEOBJECT
#include "Shape.h"
#include "btBulletDynamicsCommon.h"

class GameObject {
public:
	GameObject();

	Shape* GetShape();

	void RenderFrame();
	void UpdatePhysics();
	void SetShape(Shape* shape);
	void AddRigidbody();
private:
	Shape* shape;
	btRigidBody* rigidbody;
};
#endif

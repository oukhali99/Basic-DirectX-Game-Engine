#include "Component.h"
#include "btBulletDynamicsCommon.h"

class GameObject;

class Rigidbody : public Component {
public:
	Rigidbody(GameObject* gameObject);

	void Update() override;
	void SetMass(btScalar mass);
	void ApplyForce(btVector3 force);
private:
	btRigidBody* rigidbody;
};

#include "Component.h"
#include "btBulletDynamicsCommon.h"

class GameObject;

class Rigidbody : public Component {
public:
	Rigidbody(GameObject* gameObject);

	void Update() override;
	void SetMass(btScalar mass);
	void ApplyImpulse(btVector3 force);
	void ApplyTorqueImpulse(btVector3 torque);
	void SetIsKinematic(bool isKinematic);
	void SetGravity(btVector3 gravity);
private:
	btRigidBody* rigidbody;
	bool isKinematic;
};

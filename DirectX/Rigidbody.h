#include "Component.h"
#include "bullet/btBulletDynamicsCommon.h"

class GameObject;

class Rigidbody : public Component {
public:
	Rigidbody(GameObject* gameObject);

	btVector3 GetLinearVelocity();

	void Update() override;
	void SetMass(btScalar mass);
	void ApplyImpulse(btVector3 force);
	void ApplyTorqueImpulse(btVector3 torque);
	void SetIsKinematic(bool isKinematic);
	void SetGravity(btVector3 gravity);
	void SetFriction(btScalar friction);
	void SetAngularFactor(btVector3 factor);
private:
	btRigidBody* rigidbody;
	bool isKinematic;
};

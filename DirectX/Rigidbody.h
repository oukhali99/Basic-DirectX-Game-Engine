#include "Component.h"
#include "btBulletDynamicsCommon.h"

class GameObject;

class Rigidbody : public Component {
public:
	Rigidbody(GameObject* gameObject);

	void Update() override;
	void SetMass(btScalar mass);
	void ApplyImpulse(btVector3 force);
	void SetIsKinematic(bool isKinematic);
private:
	btRigidBody* rigidbody;
	bool isKinematic;
};

#include "Component.h"
#include "btBulletDynamicsCommon.h"

class GameObject;

class Rigidbody : public Component {
public:
	Rigidbody(GameObject* gameObject);

	void Update() override;
	void SetMass(btScalar mass);
private:
	btRigidBody* rigidbody;
};

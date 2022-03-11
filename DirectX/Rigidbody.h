#include "Component.h"
#include "btBulletDynamicsCommon.h"

class GameObject;

class Rigidbody : public Component {
public:
	Rigidbody(GameObject* gameObject);

	void Update() override;
private:
	btRigidBody* rigidbody;
};

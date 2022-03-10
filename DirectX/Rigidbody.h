#include "Component.h"
#include "btBulletDynamicsCommon.h"

class Rigidbody : public Component {
public:
	Rigidbody();
private:
	btCollisionObject* collisionObject;
};

#include "btBulletDynamicsCommon.h"

class Physics {
public:
	Physics(btDiscreteDynamicsWorld** dynamicsWorld);
	~Physics();
private:
	btDiscreteDynamicsWorld* dynamicsWorld;
};

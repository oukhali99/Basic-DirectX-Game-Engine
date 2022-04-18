#include "Component.h"
#include "GameObject.h"

class PositionConstraint : public Component {
public:
	PositionConstraint(GameObject* gameObject);

	void Update() override;
	void SetConstrainer(GameObject* constrainer);
private:
	GameObject* constrainer;
	btVector3 distance;
};

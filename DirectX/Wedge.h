#include "ShapeBase.h"

class Wedge : public ShapeBase<Wedge> {
public:
	Wedge(GameObject* gameObject);
protected:
	void SetupVertices() override;
	void SetupIndices() override;
};

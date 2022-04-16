#ifndef H_CAMERA
#define H_CAMERA
#include "Component.h"
#include "Main.h"

using namespace dx;

class Camera : public Component {
public:
	Camera(GameObject* gameObject);

	dx::XMMATRIX GetMatrix();

	void Update() override;
};
#endif

#ifndef H_CAMERA
#define H_CAMERA
#include "Component.h"

class Camera : public Component {
public:
	Camera(GameObject* gameObject);

	void Update() override;
};
#endif

#include <functional>
#include "Component.h"
#include "GameObject.h"

class InputController : public Component {
public:
	InputController(GameObject* gameObject);

	void Update() override;
	void SetOnButtonPressed(std::function<void(GameObject* gameObject, char button)> onButtonPressed);
private:
	std::function<void(GameObject* gameObject, char button)> onButtonPressed;
};

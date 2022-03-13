#include <functional>
#include "Component.h"
#include "GameObject.h"

using namespace std;

class Script : public Component {
public:
	Script(GameObject* gameObject);

	void Update() override;
	void SetOnUpdate(function<void(GameObject* gameObject)> onUpdate);
private:
	function<void(GameObject* gameObject)> onUpdate;
};

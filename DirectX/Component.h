#ifndef H_COMPONENT
#define H_COMPONENT
class GameObject;

class Component {
public:
	Component(Component& component) = delete;

	GameObject* GetGameObject();

	virtual void Update() = 0;
protected:
	Component(GameObject* gameObject);

	GameObject* gameObject;
};
#endif

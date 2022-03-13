#ifndef H_GAMEOBJECT
#define H_GAMEOBJECT
#include <vector>
#include <type_traits>
#include "Shape.h"
#include "btBulletDynamicsCommon.h"

class Component;
class Script;

class GameObject {
public:
	GameObject();
	GameObject(btTransform transform, btVector3 scale);

	btVector3 GetScale();
	btTransform GetTransform();

	void SetTransform(btTransform transform);
	void Update();

	template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	T* GetComponent() {
		for (Component* component : components) {			
			if (dynamic_cast<T*>(component)) {
				return static_cast<T*>(component);
			}
		}
		return NULL;
	}

	template<typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	void AddComponent() {
		T* t = new T(this);
		components.push_back(t);

		if (std::is_same<T, Script>::value) {
			inputControllers.push_back((Script*)(t));
		}
	}
private:
	btTransform transform;
	btVector3 scale;
	std::vector<Component*> components;
	std::vector<Script*> inputControllers;
};
#endif

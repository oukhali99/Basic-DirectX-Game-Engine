#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject* gameObject)
	:
	gameObject(gameObject)
{}

GameObject* Component::GetGameObject() {
	return gameObject;
}

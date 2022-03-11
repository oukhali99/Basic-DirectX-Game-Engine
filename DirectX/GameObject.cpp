#include "GameObject.h"
#include "Game.h"
#include "btBulletDynamicsCommon.h"
#include "Component.h"

GameObject::GameObject()
	:
	transform(btTransform()),
	scale(btVector3(1, 1, 1))
{
	Game::GetInstance()->AddGameObject(this);
}

GameObject::GameObject(btTransform transform, btVector3 scale) 
	:
	transform(transform),
	scale(scale)
{
	Game::GetInstance()->AddGameObject(this);
}

btVector3 GameObject::GetScale() {
	return scale;
}

btTransform GameObject::GetTransform() {
	return transform;
}

void GameObject::SetTransform(btTransform transform) {
	this->transform = transform;
}

void GameObject::Update() {
	for (Component* component : components) {
		component->Update();
	}
}

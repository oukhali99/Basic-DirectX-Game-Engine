#include <Windows.h>
#include "Script.h"

Script::Script(GameObject* gameObject)
	:
	Component(gameObject),
	onUpdate([](GameObject* gameObject)->void {})
{}

void Script::Update() {
	onUpdate(gameObject);
}

void Script::SetOnUpdate(function<void(GameObject* gameObject)> onUpdate) {
	this->onUpdate = onUpdate;
}

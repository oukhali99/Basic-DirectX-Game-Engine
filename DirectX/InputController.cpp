#include <Windows.h>
#include "InputController.h"
#include "Keyboard.h"

InputController::InputController(GameObject* gameObject)
	:
	Component(gameObject),
	onButtonPressed([](GameObject* gameObject, char button)->void {})
{}

void InputController::Update() {
	for (WPARAM button : *Keyboard::GetInstance()->GetPressedKeys()) {
		onButtonPressed(gameObject, button);
	}
}

void InputController::SetOnButtonPressed(std::function<void(GameObject* gameObject, char button)> onButtonPressed) {
	this->onButtonPressed = onButtonPressed;
}

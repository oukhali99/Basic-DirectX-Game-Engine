#include "InputController.h"

InputController::InputController(GameObject* gameObject)
	:
	Component(gameObject)
{}

void InputController::Update() {}

void InputController::ButtonPressed(char button) {
	onButtonPressed(gameObject, button);
}

void InputController::SetOnButtonPressed(std::function<void(GameObject* gameObject, char button)> onButtonPressed) {
	this->onButtonPressed = onButtonPressed;
}

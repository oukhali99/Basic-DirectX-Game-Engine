#include "Keyboard.h"

Keyboard* Keyboard::GetInstance() {
	if (!instance) {
		instance = new Keyboard();
	}

	return instance;
}

Keyboard::Keyboard() {}

list<WPARAM>* Keyboard::GetPressedKeys() {
	return &pressedKeys;
}

void Keyboard::InputStarted(WPARAM wParam) {
	pressedKeys.push_back(wParam);
}

void Keyboard::InputStopped(WPARAM wParam) {
	pressedKeys.remove(wParam);
}

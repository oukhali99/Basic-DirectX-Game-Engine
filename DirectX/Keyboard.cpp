#include "Keyboard.h"

Keyboard* Keyboard::GetInstance() {
	if (!instance) {
		instance = new Keyboard();
	}

	return instance;
}

Keyboard::Keyboard() {}

set<WPARAM>* Keyboard::GetPressedKeys() {
	return &pressedKeys;
}

void Keyboard::InputStarted(WPARAM wParam) {
	pressedKeys.insert(wParam);
}

void Keyboard::InputStopped(WPARAM wParam) {
	pressedKeys.erase(wParam);
}

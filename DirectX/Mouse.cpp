#include "Mouse.h"

Mouse* Mouse::GetInstance() {
	return instance;
}

void Mouse::Init(HWND hWnd) {
	instance = new Mouse(hWnd);
}

Mouse::Position Mouse::GetPosition() {
	POINT p;

	GetCursorPos(&p);
	ScreenToClient(hWnd, &p);

	return { 
		((float)p.x / SCREEN_WIDTH) - 0.5f,
		-((float)p.y / SCREEN_HEIGHT) + 0.5f
	};
}

Mouse::RawInput Mouse::GetRawInput() {
	return rawInput;
}

void Mouse::SetRawInput(RawInput rawInput) {
	this->rawInput = rawInput;
}

Mouse::Mouse(HWND hWnd)
	:
	hWnd(hWnd)
{}

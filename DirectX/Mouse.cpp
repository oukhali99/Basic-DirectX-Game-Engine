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

Mouse::RawInput* Mouse::GetRawInput() {
	return rawInput;
}

void Mouse::OnRawInput(float x, float y) {
	this->rawInput->x += x;
	this->rawInput->y += y;
}

void Mouse::SetRawInput(float x, float y) {
	this->rawInput->x = x;
	this->rawInput->y = y;
}

Mouse::Mouse(HWND hWnd)
	:
	hWnd(hWnd),
	rawInput(new RawInput({ 0, 0 }))
{}

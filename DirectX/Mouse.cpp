#include "Mouse.h"

Mouse Mouse::GetSingleton(HWND hWnd) {
	static Mouse mouse(hWnd);
	return mouse;
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

Mouse::Mouse(HWND hWnd)
	:
	hWnd(hWnd)
{}

#ifndef H_MOUSE
#define H_MOUSE
#include "Main.h"

class Mouse {
public:
	struct Position {
		float x, y;
	};

	static Mouse GetSingleton(HWND hWnd);
	Position GetPosition();
private:
	Mouse(HWND hWnd);
	HWND hWnd;
};
#endif
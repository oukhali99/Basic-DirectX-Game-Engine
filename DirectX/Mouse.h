#ifndef H_MOUSE
#define H_MOUSE
#include "Main.h"

class Mouse {
public:
	struct Position {
		float x, y;
	};

	struct RawInput {
		int x, y;
	};

	static Mouse* GetInstance();
	static void Init(HWND hWnd);

	Position GetPosition();
	RawInput* GetRawInput();

	void OnRawInput(float x, float y);
	void SetRawInput(float x, float y);
private:
	Mouse(HWND hWnd);

	inline static Mouse* instance;

	HWND hWnd;
	RawInput* rawInput;
};
#endif
#include <Windows.h>

class Window {
public:
	static void Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	static Window* GetInstance();

	HWND GetHandle();

private:
	Window(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

	inline static Window* instance;

	HWND hWnd;
};

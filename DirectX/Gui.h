#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class Gui {
public:
	static void Init(HWND hWnd);
	static Gui* GetInstance();

	void Update();
private:
	Gui(HWND hWnd);
	inline static Gui* instance;

	HWND hWnd;
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};

#include "Main.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class Gui {
public:
	static void Init(HWND hWnd);
	static Gui* GetInstance();

	ImVec4 GetBackgroundColor();

	void Update();
private:
	Gui(HWND hWnd);
	inline static Gui* instance;

	HWND hWnd;
	bool showDemoWindow;
	bool showAnotherWindow;
	ImVec4 backgroundColor;
	float nearZ, farZ;
};

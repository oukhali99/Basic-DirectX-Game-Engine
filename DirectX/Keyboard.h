#include <set>
#include <Windows.h>

using namespace std;

class Keyboard {
public:
	static Keyboard* GetInstance();

	set<WPARAM>* GetPressedKeys();

	void InputStarted(WPARAM wParam);
	void InputStopped(WPARAM wParam);
private:
	Keyboard();
	inline static Keyboard* instance;

	set<WPARAM> pressedKeys;
};

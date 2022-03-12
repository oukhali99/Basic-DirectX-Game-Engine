#include <list>
#include <Windows.h>

using namespace std;

class Keyboard {
public:
	static Keyboard* GetInstance();

	list<WPARAM>* GetPressedKeys();

	void InputStarted(WPARAM wParam);
	void InputStopped(WPARAM wParam);
private:
	Keyboard();
	inline static Keyboard* instance;

	list<WPARAM> pressedKeys;
};

#include <Windows.h>
#include <vector>

#include "Graphics.h"
#include "Physics.h"
#include "GameObject.h"
#include "Camera.h"

class Game {
public:
	static void Init(HWND hWnd);
	static Game* GetInstance();

	float GetLastUpdateTime();

	void AddGameObject(GameObject* gameObject);
	void Update();
	void SetMainCamera(Camera* camera);

	std::vector<GameObject*> GetGameObjects();
	Camera* GetMainCamera();

	Game(Game& game) = delete;
private:
	Game(HWND hWnd);
	inline static Game* instance;

	std::vector<GameObject*> gameObjects;
	float lastUpdateTime;
	Camera* mainCamera;
};

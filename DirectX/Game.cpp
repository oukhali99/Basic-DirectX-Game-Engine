#include "Game.h"
#include "btBulletDynamicsCommon.h"
#include "Clock.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "Gui.h"

void Game::Init(HWND hWnd) {
	instance = new Game(hWnd);
}

Game::Game(HWND hWnd) 
	:
	lastUpdateTime(Clock::GetSingleton().GetTimeSinceStart())
{}

Game* Game::GetInstance() {
	return instance;
}

void Game::AddGameObject(GameObject* gameObject) {
	gameObjects.push_back(gameObject);
}

void Game::SetMainCamera(Camera* camera) {
	mainCamera = camera;
}

std::vector<GameObject*> Game::GetGameObjects() {
	return gameObjects;
}

Camera* Game::GetMainCamera() {
	return mainCamera;
}

float Game::GetLastUpdateTime() {
	return lastUpdateTime;
}

void Game::Update() {
	Physics::GetInstance()->Update();

	Graphics::GetInstance()->ClearFrame();
	Graphics::GetInstance()->BindLightingBuffer();
	Graphics::GetInstance()->GenerateShadowMap();

	for (GameObject* gameObject : gameObjects) {
		gameObject->Update();
	}
	lastUpdateTime = Clock::GetSingleton().GetTimeSinceStart();

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	Gui::GetInstance()->Update();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	Graphics::GetInstance()->RenderFrame();
}

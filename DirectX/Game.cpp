#include "Game.h"
#include "btBulletDynamicsCommon.h"
#include "Clock.h"

void Game::Init(HWND hWnd) {
	instance = new Game(hWnd);
}

Game::Game(HWND hWnd) 
{}

Game* Game::GetInstance() {
	return instance;
}

void Game::AddGameObject(GameObject* gameObject) {
	gameObjects.push_back(gameObject);
}

std::vector<GameObject*> Game::GetGameObjects() {
	return gameObjects;
}

void Game::Update() {
	Physics::GetInstance()->Update();

	Graphics::GetInstance()->ClearFrame();
	for (GameObject* gameObject : gameObjects) {
		gameObject->Update();
	}
	Graphics::GetInstance()->RenderFrame();
}

void Game::ButtonPressed(char button) {
	for (GameObject* gameObject : gameObjects) {
		gameObject->ButtonPressed(button);
	}
}

#include "Light.h"
#include "Graphics.h"

Light::Light(GameObject* gameObject)
	:
	Component(gameObject)
{
	Graphics::GetInstance()->AddLight(this);
}

void Light::Update() {}

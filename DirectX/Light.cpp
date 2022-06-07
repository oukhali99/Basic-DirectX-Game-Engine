#include "Light.h"
#include "Graphics.h"
#include "GameObject.h"

Light::Light(GameObject* gameObject)
	:
	Component(gameObject)
{
	Graphics::GetInstance()->AddLight(this);
}

void Light::Update() {
	btVector3 origin = gameObject->GetTransform().getOrigin();
	float newPosition[4] = { origin.getX(), origin.getY(), origin.getZ() };

	lightData.SetPosition(newPosition);
}

void Light::LightData::SetPosition(float newPosition[4]) {
	lightPos[0] = newPosition[0];
	lightPos[1] = newPosition[1];
	lightPos[2] = newPosition[2];
	lightPos[3] = newPosition[3];
}

void Light::LightData::SetDiffuseColor(float diffuseColor[4]) {
	this->diffuseColor[0] = diffuseColor[0];
	this->diffuseColor[1] = diffuseColor[1];
	this->diffuseColor[2] = diffuseColor[2];
	this->diffuseColor[3] = diffuseColor[3];
}

void Light::LightData::SetDiffuseIntensity(float diffuseIntensity) {
	this->diffuseIntensity = diffuseIntensity;
}

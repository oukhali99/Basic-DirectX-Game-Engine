#ifndef LIGHT_H
#define LIGHT_H

#include "Component.h"

class Light : public Component {
public:
	Light(GameObject* gameObject);

	void Update() override;

	struct LightData {
        float lightPos[3] = { 0, 1, 0 };
        float ambient[3] = { 0.05f, 0.05f, 0.05f };
        float diffuseColor[3] = { 1, 1, 1 };
        float diffuseIntensity = 1;
        float attConst = 1;
        float attLin = 0.045f;
        float attQuad = 0.0075f;
        float padding[3] = { 0, 1, 0 };
	} lightData;
};

#endif

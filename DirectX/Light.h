#ifndef LIGHT_H
#define LIGHT_H

#include "Component.h"

class Light : public Component {
public:
	Light(GameObject* gameObject);

	void Update() override;

	struct LightData {
        float lightPos[4] = { 0, 1, 0, 0 };
        float ambient[4] = { 0.05f, 0.05f, 0.05f, 0 };
        float diffuseColor[4] = { 1, 1, 1, 0 };
        float diffuseIntensity = 1;
        float attConst = 1;
        float attLin = 0.045f;
        float attQuad = 0.0075f;

        void SetPosition(float newPostition[4]);
	} lightData;
};

#endif

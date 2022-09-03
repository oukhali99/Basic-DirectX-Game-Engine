#ifndef LIGHT_H
#define LIGHT_H

#include "Component.h"

class Light : public Component {
public:
	Light(GameObject* gameObject);

	void Update() override;

	struct LightData {
    public:
        void SetPosition(float newPosition[4]);
        void SetDiffuseColor(float diffuseColor[4]);
        void SetDiffuseIntensity(float diffuseIntensity);
    private:
        float lightPos[4] = { 0, 0, 0, 0 };
        float ambient[4] = { 0, 0, 0, 0 };
        float diffuseColor[4] = { 1, 1, 1, 0 };
        float diffuseIntensity = 0.1f;
        float attConst = 1;
        float attLin = 0.045f;
        float attQuad = 0.0075f;
	} lightData;
};

#endif

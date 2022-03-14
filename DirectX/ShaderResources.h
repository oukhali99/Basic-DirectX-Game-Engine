#include <string>
#include <map>
#include "Bindable.h"

class Shape;
class Texture;

class ShaderResources : public Bindable {
public:
	ShaderResources(int width, int height);

	void Bind(Shape* shape) override;
private:
	ID3D11Texture2D* imageTexture;
	ID3D11Texture2D* imageTextureCopy;
	ID3D11ShaderResourceView* shaderResourceView;
	ID3D11SamplerState* samplerState;
	LPCWSTR currentShader;
	int width, height;
};

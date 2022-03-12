#include <string>
#include <map>
#include "Bindable.h"

class Shape;

struct Image {
	unsigned char* data;
	int width;
	int height;
	int channelCount;
};

class ShaderResources : public Bindable {
public:
	ShaderResources(int width, int height);

	void Bind(Shape* shape) override;
private:
	D3D11_TEXTURE2D_DESC textureDesc;
	ID3D11Texture2D* imageTexture;
	ID3D11ShaderResourceView* shaderResourceView;
	ID3D11SamplerState* samplerState;
	std::string texturePath;
	std::map<std::string, Image> textureCache;
	LPCWSTR currentShader;
	int width, height;
};

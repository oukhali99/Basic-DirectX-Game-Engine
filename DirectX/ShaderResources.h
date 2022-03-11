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
	ShaderResources(std::string texturePath);

	void Bind(Shape* shape) override;
private:
	ID3D11ShaderResourceView* image_shader_resource_view;
	ID3D11SamplerState* image_sampler_state;
	std::string texturePath;
	std::map<std::string, Image> textureCache;
};

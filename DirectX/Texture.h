#include <map>
#include <string>

using namespace std;

class Texture {
public:
	Texture(string texturePath);

	struct Image {
		unsigned char* data;
		int width;
		int height;
		int channelCount;
	} image;

private:
	inline static map<std::string, Image> textureCache;

	string texturePath;
};

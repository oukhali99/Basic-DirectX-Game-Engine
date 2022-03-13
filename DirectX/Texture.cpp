#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(string texturePath)
	:
	image({}),
    texturePath(texturePath)
{
    if (textureCache.contains(texturePath)) {
        image = textureCache[texturePath];
    }
    else {
        // Load the image
        int loadedWidth, loadedHeight;
        int channelCount = 4;
        unsigned char* imageData = stbi_load(texturePath.c_str(), &loadedWidth, &loadedHeight, NULL, channelCount);

        image.data = imageData;
        image.width = loadedWidth;
        image.height = loadedHeight;
        image.channelCount = channelCount;

        // Save to cache
        textureCache[texturePath] = image;
    }
}

#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(string texturePath)
	:
	image(0),
    texturePath(texturePath)
{
    if (textureCache[texturePath]) {
        image = textureCache[texturePath];
    }
    else {
        // Load the image
        int loadedWidth, loadedHeight;
        int channelCount = 4;
        unsigned char* imageData = stbi_load(texturePath.c_str(), &loadedWidth, &loadedHeight, NULL, channelCount);

        image = new Image();
        image->data = imageData;
        image->width = loadedWidth;
        image->height = loadedHeight;
        image->channelCount = channelCount;

        // Save to cache
        textureCache[texturePath] = image;
    }
}

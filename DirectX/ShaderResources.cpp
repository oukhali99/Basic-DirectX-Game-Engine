#include <string>
#include "ShaderResources.h"
#include "Texture.h"
#include "Graphics.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

ShaderResources::ShaderResources(int width, int height)
    :
    shaderResourceView(0),
    samplerState(0),
    texturePath(""),
    currentShader(SHADER_FILE_NAME_DEFAULT),
    width(width),
    height(height)
{
    // Create the texture
    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1u;
    textureDesc.ArraySize = 1u;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1u;
    textureDesc.SampleDesc.Quality = 0u;
    textureDesc.Usage = D3D11_USAGE_DYNAMIC;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreateTexture2D(
        &textureDesc,
        NULL,
        &imageTexture
    ));

    // Create the resource view
    GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreateShaderResourceView(imageTexture,
        nullptr,
        &shaderResourceView
    ));

    // Create the sampler state
    D3D11_SAMPLER_DESC image_sampler_desc = {};
    image_sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    image_sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    image_sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    image_sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    image_sampler_desc.MipLODBias = 0.0f;
    image_sampler_desc.MaxAnisotropy = 1;
    image_sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    image_sampler_desc.BorderColor[0] = 1.0f;
    image_sampler_desc.BorderColor[1] = 1.0f;
    image_sampler_desc.BorderColor[2] = 1.0f;
    image_sampler_desc.BorderColor[3] = 1.0f;
    image_sampler_desc.MinLOD = -FLT_MAX;
    image_sampler_desc.MaxLOD = FLT_MAX;
    
    GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreateSamplerState(&image_sampler_desc,
        &samplerState));
}

void ShaderResources::Bind(Shape* shape) {
    Texture* texture = shape->GetTexture();

    // Load the image
    /*
    Texture::Image image = {};
    if (textureCache.contains(texturePath)) {
        image = textureCache[texturePath];
    }
    else {
        // Load the image
        int loadedWidth, loadedHeight;
        int channelCount = 4;
        unsigned char* imageData = stbi_load(texturePath.c_str(), &loadedWidth, &loadedHeight, NULL, channelCount);
            
        if (imageData) {
            unsigned char* resizedImageData = (unsigned char*)malloc(width * height * channelCount);
            stbir_resize_uint8(imageData, loadedWidth, loadedHeight, 0, resizedImageData, width, height, 0, channelCount);

            // Create the image struct
            image.data = resizedImageData;
            image.width = width;
            image.height = height;
            image.channelCount = channelCount;
        }
        else {
            image.data = 0;
            image.width = 0;
            image.height = 0;
            image.channelCount = 0;
        }

        // Save to cache
        textureCache[texturePath] = image;
    }
    */

    if (texture && texture->image.data) {
        if (texture->image.width != width || texture->image.height != height) {
            unsigned char* resizedImageData = (unsigned char*)malloc(width * height * texture->image.channelCount);
            stbir_resize_uint8(
                texture->image.data,
                texture->image.width,
                texture->image.height,
                0,
                resizedImageData,
                width,
                height,
                0,
                texture->image.channelCount
            );

            free(texture->image.data);

            // Modify the image struct
            texture->image.data = resizedImageData;
            texture->image.width = width;
            texture->image.height = height;
        }

        int imageRowPitch = texture->image.width * texture->image.channelCount;

        // Modify the texture
        D3D11_MAPPED_SUBRESOURCE msr = {};
        Graphics::GetInstance()->GetDeviceContext()->Map(imageTexture, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
        BYTE* mappedData = reinterpret_cast<BYTE*>(msr.pData);
        BYTE* newTextureData = texture->image.data;
        for (UINT row = 0; row < texture->image.height; row++)
        {
            memcpy(mappedData, newTextureData, imageRowPitch);
            mappedData += msr.RowPitch;
            newTextureData += imageRowPitch;
        }
        Graphics::GetInstance()->GetDeviceContext()->Unmap(imageTexture, 0u);

        // Set the shader
        currentShader = SHADER_FILE_NAME_TEXTURE;
    }
    else {
        currentShader = SHADER_FILE_NAME_DEFAULT;
    }


    // set the shader objects
    Graphics::GetInstance()->SetShaders(currentShader);

    Graphics::GetInstance()->GetDeviceContext()->PSSetShaderResources(0, 1, &shaderResourceView);
    Graphics::GetInstance()->GetDeviceContext()->PSSetSamplers(0, 1, &samplerState);
}

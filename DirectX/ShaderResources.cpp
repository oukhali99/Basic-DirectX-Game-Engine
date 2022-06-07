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
    textureDesc.ArraySize = 6u;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1u;
    textureDesc.SampleDesc.Quality = 0u;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreateTexture2D(
        &textureDesc,
        NULL,
        &imageTexture
    ));

    textureDesc.Usage = D3D11_USAGE_STAGING;
    textureDesc.BindFlags = 0u;
    textureDesc.ArraySize = 6u;
    GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreateTexture2D(
        &textureDesc,
        NULL,
        &imageTextureCopy
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

    if (texture && texture->image->data) {
        if (texture->image->width != width || texture->image->height != height) {
            unsigned char* resizedImageData = (unsigned char*)malloc(width * height * texture->image->channelCount);
            stbir_resize_uint8(
                texture->image->data,
                texture->image->width,
                texture->image->height,
                0,
                resizedImageData,
                width,
                height,
                0,
                texture->image->channelCount
            );

            free(texture->image->data);

            // Modify the image struct
            texture->image->data = resizedImageData;
            texture->image->width = width;
            texture->image->height = height;
        }

        int imageRowPitch = texture->image->width * texture->image->channelCount;

        // Modify the texture copy
        for (int i = 0; i < 1; i++) {
            D3D11_MAPPED_SUBRESOURCE msr = {};
            Graphics::GetInstance()->GetDeviceContext()->Map(imageTextureCopy, i, D3D11_MAP_WRITE, 0u, &msr);
            BYTE* mappedData = reinterpret_cast<BYTE*>(msr.pData);
            BYTE* newTextureData = texture->image->data;
            for (UINT row = 0; row < texture->image->height; row++)
            {
                memcpy(mappedData, newTextureData, imageRowPitch);
                mappedData += msr.RowPitch;
                newTextureData += imageRowPitch;
            }
            Graphics::GetInstance()->GetDeviceContext()->Unmap(imageTextureCopy, i);
        }

        // Move the texture copy to the texture
        Graphics::GetInstance()->GetDeviceContext()->CopyResource(imageTexture, imageTextureCopy);

        // Set the shader
        currentShader = SHADER_FILE_NAME_TEXTURE;
    }

    Graphics::GetInstance()->GetDeviceContext()->PSSetShaderResources(0, 1, &shaderResourceView);
    Graphics::GetInstance()->GetDeviceContext()->PSSetSamplers(0, 1, &samplerState);
}

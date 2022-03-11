#include "ShaderResources.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Graphics.h"

ShaderResources::ShaderResources(std::string texturePath)
    :
    shaderResourceView(0),
    samplerState(0),
    texturePath(texturePath),
    currentShader(SHADER_FILE_NAME_DEFAULT)
{
    // Create the texture
    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = 800;
    textureDesc.Height = 825;
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
    image_sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    image_sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    image_sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    image_sampler_desc.MipLODBias = 0.0f;
    image_sampler_desc.MaxAnisotropy = 1;
    image_sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    image_sampler_desc.BorderColor[0] = 1.0f;
    image_sampler_desc.BorderColor[1] = 1.0f;
    image_sampler_desc.BorderColor[2] = 1.0f;
    image_sampler_desc.BorderColor[3] = 1.0f;
    image_sampler_desc.MinLOD = -FLT_MAX;
    image_sampler_desc.MaxLOD = FLT_MAX;
    if (samplerState) {
        samplerState->Release();
        samplerState = 0;
    }
    GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreateSamplerState(&image_sampler_desc,
        &samplerState));
}

void ShaderResources::Bind(Shape* shape) {
    std::string newTexturePath = shape->GetTexturePath();
    if (newTexturePath != texturePath) {
        texturePath = newTexturePath;

        // Load the image
        Image image = {};
        if (textureCache.contains(texturePath)) {
            image = textureCache[texturePath];
        }
        else {
            // Load the image
            int width, height, channelCount;
            unsigned char* imageData = stbi_load(texturePath.c_str(), &width, &height, &channelCount, 4);

            // Create the image struct
            image.data = imageData;
            image.width = width;
            image.height = height;
            image.channelCount = channelCount;

            // Save to cache
            textureCache[texturePath] = image;
        }

        if (image.data) {
            int imagePitch = image.width * sizeof(float);

            // Modify the texture
            D3D11_MAPPED_SUBRESOURCE msr = {};
            Graphics::GetInstance()->GetDeviceContext()->Map(imageTexture, 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
            BYTE* mappedData = reinterpret_cast<BYTE*>(msr.pData);
            for (UINT i = 0; i < image.height; i++)
            {
                memcpy(mappedData, image.data, image.width * sizeof(float));
                mappedData += msr.RowPitch;
                image.data += image.width * sizeof(float);
            }
            Graphics::GetInstance()->GetDeviceContext()->Unmap(imageTexture, 0u);

            // Set the shader
            currentShader = SHADER_FILE_NAME_TEXTURE;
        }
        else {
            currentShader = SHADER_FILE_NAME_DEFAULT;
        }
    }


    // set the shader objects
    Graphics::GetInstance()->SetShaders(currentShader);

    Graphics::GetInstance()->GetDeviceContext()->PSSetShaderResources(0, 1, &shaderResourceView);
    Graphics::GetInstance()->GetDeviceContext()->PSSetSamplers(0, 1, &samplerState);
}

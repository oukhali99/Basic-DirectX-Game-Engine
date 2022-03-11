#include "ShaderResources.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Graphics.h"

ShaderResources::ShaderResources(std::string texturePath)
    :
    shaderResourceView(0),
    samplerState(0),
    texturePath(texturePath)
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
}

void ShaderResources::Bind(Shape* shape) {
    std::string newTexturePath = shape->GetTexturePath();
    if (newTexturePath != texturePath) {
        texturePath = newTexturePath;

        // Reset these
        shaderResourceView = 0;
        samplerState = 0;

        Image image = {};
        if (textureCache.contains(texturePath)) {
            image = textureCache.at(texturePath);
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
        int imagePitch = image.width * sizeof(float);

        if (image.data) {
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
            GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreateSamplerState(&image_sampler_desc,
                &samplerState));
            Graphics::GetInstance()->GetDeviceContext()->PSSetShaderResources(0, 1, &shaderResourceView);
            Graphics::GetInstance()->GetDeviceContext()->PSSetSamplers(0, 1, &samplerState);
        }
    }


    

    if (shaderResourceView && samplerState) {
        // load and compile the two shaders
        ID3DBlob* VS = NULL;
        ID3DBlob* PS = NULL;
        ID3DBlob* errorBlob = NULL;
        ID3D11VertexShader* pVS;                    // the vertex shader
        ID3D11PixelShader* pPS;                     // the pixel shader

        GFX_THROW_INFO(D3DCompileFromFile(L"TextureShaders.shaders", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, &errorBlob));
        GFX_THROW_INFO(D3DCompileFromFile(L"TextureShaders.shaders", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, &errorBlob));

        GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS));
        GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS));

        // set the shader objects
        Graphics::GetInstance()->GetDeviceContext()->VSSetShader(pVS, 0, 0);
        Graphics::GetInstance()->GetDeviceContext()->PSSetShader(pPS, 0, 0);

        pVS->Release();
        pPS->Release();
    }
    else {
        // load and compile the two shaders
        ID3DBlob* VS = NULL;
        ID3DBlob* PS = NULL;
        ID3DBlob* errorBlob = NULL;
        ID3D11VertexShader* pVS;                    // the vertex shader
        ID3D11PixelShader* pPS;                     // the pixel shader

        GFX_THROW_INFO(D3DCompileFromFile(L"DefaultShaders.shaders", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, &errorBlob));
        GFX_THROW_INFO(D3DCompileFromFile(L"DefaultShaders.shaders", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, &errorBlob));

        GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS));
        GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS));

        // set the shader objects
        Graphics::GetInstance()->GetDeviceContext()->VSSetShader(pVS, 0, 0);
        Graphics::GetInstance()->GetDeviceContext()->PSSetShader(pPS, 0, 0);

        pVS->Release();
        pPS->Release();
    }

    
}

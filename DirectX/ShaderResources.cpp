#include "ShaderResources.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Graphics.h"

ShaderResources::ShaderResources(std::string texturePath)
    :
    image_shader_resource_view(0),
    image_sampler_state(0),
    texturePath(texturePath)
{}

void ShaderResources::Bind(Shape* shape) {
    std::string newTexturePath = shape->GetTexturePath();
    if (newTexturePath != texturePath) {
        texturePath = newTexturePath;

        // Reset these
        image_shader_resource_view = 0;
        image_sampler_state = 0;

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
        int image_pitch = image.width * sizeof(float);

        if (image.data) {
            // Create the texture
            D3D11_TEXTURE2D_DESC image_texture_desc;
            ZeroMemory(&image_texture_desc, sizeof(image_texture_desc));
            image_texture_desc.Width = image.width;
            image_texture_desc.Height = image.height;
            image_texture_desc.MipLevels = 1u;
            image_texture_desc.ArraySize = 1u;
            image_texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            image_texture_desc.SampleDesc.Count = 1u;
            image_texture_desc.SampleDesc.Quality = 0u;
            image_texture_desc.Usage = D3D11_USAGE_IMMUTABLE;
            image_texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

            D3D11_SUBRESOURCE_DATA image_subresource_data = {};
            image_subresource_data.pSysMem = image.data;
            image_subresource_data.SysMemPitch = image_pitch;

            ID3D11Texture2D* image_texture;
            GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreateTexture2D(
                &image_texture_desc,
                &image_subresource_data,
                &image_texture
            ));

            //ID3D11ShaderResourceView* image_shader_resource_view;
            GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreateShaderResourceView(image_texture,
                nullptr,
                &image_shader_resource_view
            ));

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

            //ID3D11SamplerState* image_sampler_state;
            GFX_THROW_INFO(Graphics::GetInstance()->GetDevice()->CreateSamplerState(&image_sampler_desc,
                &image_sampler_state));

            Graphics::GetInstance()->GetDeviceContext()->PSSetShaderResources(0, 1, &image_shader_resource_view);
            Graphics::GetInstance()->GetDeviceContext()->PSSetSamplers(0, 1, &image_sampler_state);
        }
    }


    

    if (image_shader_resource_view && image_sampler_state) {
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

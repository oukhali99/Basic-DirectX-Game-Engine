#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <string>
#include <sstream>

namespace dx = DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

#define GFX_THROW_INFO(hrcall) if (FAILED(hr = (hrcall))) { HandleError(hr, __FILE__, __LINE__); }

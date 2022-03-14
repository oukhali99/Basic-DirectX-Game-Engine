#ifndef H_MAIN
#define H_MAIN
#include <vector>
#include <cmath>
#include <exception>
#include <stdexcept>
#include <comdef.h>
#include <string>
#include <chrono>
#include <sstream>
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <windef.h>

namespace dx = DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

#define GFX_THROW_INFO(hrcall) if (FAILED(hr = (hrcall))) { Main::HandleError(hr, __FILE__, __LINE__); }

class Main {
public:
    static void HandleError(HRESULT hr, const char* file, const long long line) {
        std::stringstream ss;

        // Exception line
        ss << "Exception on line " << line << std::endl;

        // Exception file
        ss << "File: " << file << std::endl;

        // Error code
        ss << "Error code: " << "0x" << std::hex << hr << std::endl;

        // Error description
        char errorMessage[50];
        FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            hr,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            errorMessage,
            sizeof(errorMessage) / sizeof(errorMessage[0]),
            NULL
        );

        ss << "Description: " << errorMessage << std::endl;

        throw new std::exception(ss.str().c_str());
    }

    static void HandleError(std::string description, const char* file, const long long line) {
        std::stringstream ss;

        // Exception line
        ss << "Exception on line " << line << std::endl;

        // Exception file
        ss << "File: " << file << std::endl;


        ss << "Description: " << description << std::endl;

        throw new std::exception(ss.str().c_str());
    }
};
#endif

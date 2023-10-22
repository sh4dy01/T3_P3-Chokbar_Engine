#pragma once

#ifdef WIN32
	#include <windows.h>
#endif

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <Core/d3dx12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
using namespace DirectX;

#include <string>

#include "Core/Core.h"
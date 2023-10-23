#pragma once

#include "MeshGeometry.h"

inline CHOKBAR_API std::wstring AnsiToWString(const std::string& str)
{
    WCHAR buffer[512];
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
    return std::wstring(buffer);
}

#ifndef ThrowIfFailed
class CHOKBAR_API DxException
{
public:

    DxException() = default;
    DxException(HRESULT hr, const std::wstring& functionName,
        const std::wstring& filename, int lineNumber);

    std::wstring ToString()const;

    HRESULT ErrorCode = S_OK;
    std::wstring FunctionName;
    std::wstring Filename;
    int LineNumber = -1;

};

#define ThrowIfFailed(x)                                              \
{                                                                     \
    HRESULT hr__ = (x);                                               \
    std::wstring wfn = AnsiToWString(__FILE__);                       \
    if(FAILED(hr__)) { throw DxException(hr__, L#x, wfn, __LINE__); } \
}
#endif

#ifndef ReleaseCom
	#define ReleaseCom(x) { if(x){ x->Release(); x = 0; } }
#endif

/* ------------------------------------------------------------------------- */
/* GLOBAL VARIABLES                                                          */
/* ------------------------------------------------------------------------- */
#pragma region GlobalVariables
static const int SWAP_CHAIN_BUFFER_COUNT = 2;
static const int FRAME_RESOURCE_COUNT = 3;
#pragma endregion

/* ------------------------------------------------------------------------- */
/* HELPER FONCTIONS                                                          */
/* ------------------------------------------------------------------------- */
#pragma region HelperFunctions
XMFLOAT4X4 Identity4x4();
#pragma endregion

/* ------------------------------------------------------------------------- */
/* HELPER STRUCTS                                                            */
/* ------------------------------------------------------------------------- */
#pragma region Helper Structs
enum TRANSFORMATION_TYPE { TRANSLATION, ROTATION, SCALE };
	
struct CHOKBAR_API Vertex
{
	XMFLOAT3 Pos;
	UINT32 Color;
};

struct CHOKBAR_API ObjectConstants
{
	XMFLOAT4X4 World = Identity4x4();
};

struct CHOKBAR_API PassConstants
{
	XMFLOAT4X4 View = Identity4x4();
	XMFLOAT4X4 InvView = Identity4x4();
	XMFLOAT4X4 Proj = Identity4x4();
	XMFLOAT4X4 InvProj = Identity4x4();
	XMFLOAT4X4 ViewProj = Identity4x4();
	XMFLOAT4X4 InvViewProj = Identity4x4();
	
	XMFLOAT3 EyePosW = { 0.0f, 0.0f, 0.0f };
	float cbPerObjectPad1 = 0.0f;
	XMFLOAT2 RenderTargetSize = { 0.0f, 0.0f };
	XMFLOAT2 InvRenderTargetSize = { 0.0f, 0.0f };
	float NearZ = 0.0f;
	float FarZ = 0.0f;
	float TotalTime = 0.0f;
	float DeltaTime = 0.0f;
};

struct CHOKBAR_API RenderItem
{

	RenderItem() = default;
	
	XMFLOAT4X4 World = Identity4x4();

	UINT NumFramesDirty = FRAME_RESOURCE_COUNT;
	UINT ObjCBIndex = -1;

	// MeshGeometry associated with this render item
	MeshGeometry* Geo = nullptr;

	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	INT BaseVertexLocation = 0;

	TRANSFORMATION_TYPE TransformationType = TRANSLATION;
};
#pragma endregion
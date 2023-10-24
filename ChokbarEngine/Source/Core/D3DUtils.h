#pragma once

inline std::wstring AnsiToWString(const std::string& str)
{
    WCHAR buffer[512];
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
    return std::wstring(buffer);
}

#ifndef ThrowIfFailed
class DxException
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
/* HELPER FONCTIONS                                                          */
/* ------------------------------------------------------------------------- */
#pragma region HelperFunctions
inline CHOKBAR_API XMFLOAT4X4 Identity4x4();
#pragma endregion

/* ------------------------------------------------------------------------- */
/* HELPER STRUCTS                                                            */
/* ------------------------------------------------------------------------- */
#pragma region Helper Structs
	
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

struct CHOKBAR_API Camera
{
	XMFLOAT3 Position = { 0.0f, 3.0f, -6.0f };
	XMFLOAT3 Look = { 0.0f, 0.0f, 1.0f };
	XMFLOAT3 Up = { 0.0f, 1.0f, 0.0f };

	float NearZ = 0.05F;
	float FarZ = 1000.0F;
	float Aspect = 0.0f;

	XMFLOAT4X4 View = Identity4x4();
	XMFLOAT4X4 Proj = Identity4x4();
	XMFLOAT4X4 ViewProj = Identity4x4();

	void UpdateViewMatrix();
	void UpdateProjMatrix(const float winWidth, const float winHeight);
};
#pragma endregion
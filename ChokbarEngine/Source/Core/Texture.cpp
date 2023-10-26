#include "Chokbar.h"
#include "Texture.h"

#include "External/DDSTextureLoader.h"
#include <wrl.h>

using namespace Microsoft::WRL;
using namespace DirectX;

Texture::Texture(const std::string& filename, const std::wstring& filepath) : IResourceObject(filepath), Name(filename)
{
	Resource = nullptr;
	UploadHeap = nullptr;
}

Texture::~Texture()
{
	UploadHeap->Release();
	Resource->Release();
}

void Texture::Load(const std::wstring& filepath)
{
	// TODO Remove D3DApp sigleton
	D3DApp::GetInstance()->BeginList();
	LoadTexture(D3DApp::GetInstance()->GetDevice(), D3DApp::GetInstance()->GetCommandList());
	D3DApp::GetInstance()->UpdateTextureHeap(this);
	D3DApp::GetInstance()->EndList();
}

void Texture::LoadTexture(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList)
{
	// This is a bit ugly, but CreateDDSTextureFromFile12 needs ComPtr, which we do not use in the rest of the engine.
	ComPtr<ID3D12Resource> textureUploadHeap = UploadHeap;
	ComPtr<ID3D12Resource> textureResource = Resource;

	CreateDDSTextureFromFile12(device, cmdList, m_filepath.c_str(), textureResource, textureUploadHeap);

	Resource = textureResource.Detach();
	UploadHeap = textureUploadHeap.Detach();
}
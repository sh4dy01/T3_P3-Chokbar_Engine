#include "Chokbar.h"
#include "Engine/IResourceObject.h"

#include "Texture.h"

#include "Core/D3D/External/DDSTextureLoader.h"
#include <wrl.h>

using namespace Microsoft::WRL;
using namespace DirectX;

Texture::Texture(const std::string& filename) : IResourceObject(filename), HeapIndex(-1)
{
	Resource = nullptr;
	UploadHeap = nullptr;
}

Texture::~Texture()
{
	RELPTR(UploadHeap);
	RELPTR(Resource);
}

void Texture::Load(const std::string& filepath)
{
	m_filepath = filepath;

	auto app = I(D3DApp);
	app->BeginList();

	LoadTexture(app->GetDevice(), app->GetCommandList());

	app->EndList();
}

void Texture::LoadTexture(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList)
{
	// This is a bit ugly, but CreateDDSTextureFromFile12 needs ComPtr, which we do not use in the rest of the engine.
	ComPtr<ID3D12Resource> textureUploadHeap = UploadHeap;
	ComPtr<ID3D12Resource> textureResource = Resource;

	// I hate this. But idk how to convert from std::string to const wchar_t*. Please help
	const std::wstring wstr = std::wstring(m_filepath.begin(), m_filepath.end());
	CreateDDSTextureFromFile12(device, cmdList, wstr.c_str(), textureResource, textureUploadHeap);

	Resource = textureResource.Detach();
	UploadHeap = textureUploadHeap.Detach();

	HeapIndex = I(D3DApp)->UpdateTextureHeap(this);
}
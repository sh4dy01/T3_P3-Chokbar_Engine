#pragma once 

#include "Engine/IResourceObject.h"

class Texture : public IResourceObject
{
public:
	Texture(const std::string& name);
	~Texture();
	std::string Name = "";
	ID3D12Resource* Resource = nullptr;
	ID3D12Resource* UploadHeap = nullptr;

	void Load(const std::string& filepath) override;

private:
	void LoadTexture(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);
};
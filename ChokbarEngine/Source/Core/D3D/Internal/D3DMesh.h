#pragma once 

#include "Core/D3D/D3DMath.h"

enum TRANSFORMATION_TYPE { TRANSLATION, ROTATION, SCALE };

class D3DMesh
{
public:
	D3DMesh();
	~D3DMesh();

	std::string Name;

	std::vector<DirectX::XMFLOAT3> Vertices;
	std::vector<UINT> Indices;
	std::vector<DirectX::XMFLOAT2> UV;
	
public:
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const;
	D3D12_INDEX_BUFFER_VIEW IndexBufferView() const;

	void DisposeUploaders();

	void CreateMeshGPU();

private:
	ID3D12Resource* VertexBufferGPU;
	ID3D12Resource* IndexBufferGPU;

	ID3D12Resource* VertexBufferUploader;
	ID3D12Resource* IndexBufferUploader;

	UINT VertexByteStride;
	UINT VertexBufferByteSize;
	DXGI_FORMAT IndexFormat;
	UINT IndexBufferByteSize;

	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	INT BaseVertexLocation = 0;

private:
	ID3D12Resource* CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const void* initData, UINT64 byteSize, ID3D12Resource*& uploadBuffer);

};


#pragma once 

#include <unordered_map>

struct SubmeshGeometry
{
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	INT BaseVertexLocation = 0;
};

class MeshGeometry
{
public:
	MeshGeometry();
	~MeshGeometry();

	std::string Name;

	ID3DBlob* VertexBufferCPU;
	ID3DBlob* IndexBufferCPU;

	ID3D12Resource* VertexBufferGPU;
	ID3D12Resource* IndexBufferGPU;

	ID3D12Resource* VertexBufferUploader;
	ID3D12Resource* IndexBufferUploader;

	UINT VertexByteStride;
	UINT VertexBufferByteSize;
	DXGI_FORMAT IndexFormat;
	UINT IndexBufferByteSize;

	std::unordered_map<std::string, SubmeshGeometry> DrawArgs;

public:
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const;
	D3D12_INDEX_BUFFER_VIEW IndexBufferView() const;
	
	void DisposeUploaders();
};
	
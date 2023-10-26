#pragma once 

#include "D3DMath.h"

enum TRANSFORMATION_TYPE { TRANSLATION, ROTATION, SCALE };

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
};

struct Vertex_PosColor : public Vertex
{
	DirectX::XMFLOAT4 Color;
};

struct Vertex_PosTex : public Vertex
{
	DirectX::XMFLOAT2 UV;
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

	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	INT BaseVertexLocation = 0;

public:
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const;
	D3D12_INDEX_BUFFER_VIEW IndexBufferView() const;

	void DisposeUploaders();
};


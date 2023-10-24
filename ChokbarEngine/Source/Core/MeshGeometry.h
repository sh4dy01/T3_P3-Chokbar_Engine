#pragma once 

#include <unordered_map>
#include "D3DMath.h"
#include "D3DUtils.h"

enum TRANSFORMATION_TYPE { TRANSLATION, ROTATION, SCALE };

struct Vertex
{
	XMFLOAT3 Pos;
	UINT32 Color;
};

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


struct RenderItem
{
	RenderItem() = default;
	
	Transform Transform;
	XMFLOAT4X4 World = Identity4x4();

	UINT ObjCBIndex = -1;

	MeshGeometry* Geo = nullptr;

	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	INT BaseVertexLocation = 0;

	TRANSFORMATION_TYPE TransformationType = TRANSLATION;
};
	
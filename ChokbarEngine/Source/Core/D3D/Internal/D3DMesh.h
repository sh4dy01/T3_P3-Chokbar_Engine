#pragma once 

struct Vertex
{
	Vertex() {}
	Vertex(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT3& t, const DirectX::XMFLOAT2& uv);
	Vertex(
		float px, float py, float pz,
		float nx, float ny, float nz,
		float tx, float ty, float tz,
		float u, float v
	);
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT3 TangentU;
	DirectX::XMFLOAT2 TexC;
};

struct Vertex_Color
{
	Vertex_Color() {}
	Vertex_Color(const DirectX::XMFLOAT3& position, const DirectX::XMVECTORF32& color);
	Vertex_Color(
		float px, float py, float pz,
		float cr, float cg, float cb, float ca
	);
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
};

class D3DMesh
{
public:
	D3DMesh();
	~D3DMesh();

	std::string Name;

public:
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const;
	D3D12_INDEX_BUFFER_VIEW IndexBufferView() const;

	void DisposeUploaders();

	void Create(const void* vData, UINT vStride, UINT vSize, const void* iData, UINT iStride, UINT iSize);

	UINT GetIndexCount() const { return IndexCount; }

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


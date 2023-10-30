#pragma once 

struct Vertex
	{
		Vertex() {}
		Vertex(
			const DirectX::XMFLOAT3& position,
			const DirectX::XMFLOAT4& color,
			const DirectX::XMFLOAT2& uv
		);
		Vertex(
			float px, float py, float pz,
			float cr, float cg, float cb, float ca,
			float u, float v
		);
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;
		DirectX::XMFLOAT2 Texcoord;
	};

class D3DMesh
{
public:
	D3DMesh();
	~D3DMesh();

	std::string Name;

	std::vector<DirectX::XMFLOAT3> Vertices{};
	std::vector<UINT> Indices{};
	std::vector<DirectX::XMFLOAT4> Colors{};
	std::vector<DirectX::XMFLOAT2> UV{};
	
public:
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const;
	D3D12_INDEX_BUFFER_VIEW IndexBufferView() const;

	void DisposeUploaders();

	void Create(std::vector<Vertex>& vertices, std::vector<UINT>& indices);

private:
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


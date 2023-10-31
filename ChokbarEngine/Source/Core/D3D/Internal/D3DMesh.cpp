#include "Chokbar.h"

#include "Core/D3D/D3DMath.h"

#include "D3DMesh.h"

using namespace DirectX;

Vertex::Vertex(const DirectX::XMFLOAT3& position) : Position(position) { }
Vertex::Vertex(float px, float py, float pz) : Position(px, py, pz) { }

Vertex_Color::Vertex_Color(const DirectX::XMFLOAT3& position, const DirectX::XMVECTORF32& color) : Position(position), Color(color) { }
Vertex_Color::Vertex_Color(float px, float py, float pz, float cr, float cg, float cb, float ca) : Position(px, py, pz), Color(cr, cg, cb, ca) { }

Vertex_UV::Vertex_UV(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& uv) : Position(position), Texcoord(uv) { }
Vertex_UV::Vertex_UV(float px, float py, float pz, float u, float v) : Position(px, py, pz), Texcoord(u, v) { }

D3DMesh::D3DMesh()
	: VertexByteStride(0), VertexBufferByteSize(0), IndexFormat(DXGI_FORMAT_R16_UINT), IndexBufferByteSize(0)
{
	VertexBufferGPU = nullptr;
	IndexBufferGPU = nullptr;

	VertexBufferUploader = nullptr;
	IndexBufferUploader = nullptr;
}

D3DMesh::~D3DMesh()
{
	DisposeUploaders();

	RELPTR(VertexBufferGPU);
	RELPTR(IndexBufferGPU);
}

D3D12_VERTEX_BUFFER_VIEW D3DMesh::VertexBufferView() const {
	D3D12_VERTEX_BUFFER_VIEW vbv;
	vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
	vbv.StrideInBytes = VertexByteStride;
	vbv.SizeInBytes = VertexBufferByteSize;

	return vbv;
}

D3D12_INDEX_BUFFER_VIEW D3DMesh::IndexBufferView() const {
	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
	ibv.Format = IndexFormat;
	ibv.SizeInBytes = IndexBufferByteSize;

	return ibv;
}

void D3DMesh::DisposeUploaders()
{
	RELPTR(VertexBufferUploader);
	RELPTR(IndexBufferUploader);
}

void D3DMesh::Create(const void* vData, UINT vStride, UINT vSize, const void* iData, UINT iStride, UINT iSize)
{
	I(D3DApp)->BeginList();
	ID3D12Device* device = I(D3DApp)->GetDevice();
	ID3D12GraphicsCommandList* cmdList = I(D3DApp)->GetCommandList();

	const UINT iBufferSize = iStride * iSize;
	const UINT vBufferSize = vStride * vSize;

	Name = typeid(D3DMesh).name();

	VertexBufferGPU = CreateDefaultBuffer(device, cmdList, vData, vBufferSize, VertexBufferUploader);
	VertexByteStride = vStride;
	VertexBufferByteSize = vBufferSize;

	IndexBufferGPU = CreateDefaultBuffer(device, cmdList, iData, iBufferSize, IndexBufferUploader);
	IndexFormat = DXGI_FORMAT_R32_UINT;
	IndexBufferByteSize = iBufferSize;

	IndexCount = iSize;
	StartIndexLocation = 0;
	BaseVertexLocation = 0;

	I(D3DApp)->EndList();

	DisposeUploaders();
}

ID3D12Resource* D3DMesh::CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const void* initData, UINT64 byteSize, ID3D12Resource*& uploadBuffer)
{
	ID3D12Resource* defaultBuffer = nullptr;

	CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
	device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&defaultBuffer));
	if (defaultBuffer == nullptr) return nullptr;

	CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadBuffer));
	if (uploadBuffer == nullptr) return nullptr;

	D3D12_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pData = initData;
	subresourceData.RowPitch = byteSize;
	subresourceData.SlicePitch = subresourceData.RowPitch;

	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
	cmdList->ResourceBarrier(1, &barrier);
	UpdateSubresources<1>(cmdList, defaultBuffer, uploadBuffer, 0, 0, 1, &subresourceData);
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

	return defaultBuffer;
}

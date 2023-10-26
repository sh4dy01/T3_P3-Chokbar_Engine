#include "Chokbar.h"
#include "D3DMesh.h"

using namespace DirectX;

D3DMesh::D3DMesh() : VertexByteStride(0), VertexBufferByteSize(0), IndexFormat(DXGI_FORMAT_R16_UINT), IndexBufferByteSize(0)
{
	VertexBufferGPU = nullptr;
	IndexBufferGPU = nullptr;

	VertexBufferUploader = nullptr;
	IndexBufferUploader = nullptr;
}

D3DMesh::~D3DMesh()
{
	VertexBufferUploader->Release();
	IndexBufferUploader->Release();

	VertexBufferGPU->Release();
	IndexBufferGPU->Release();
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
	VertexBufferUploader->Release();
	IndexBufferUploader->Release();
}

void D3DMesh::CreateMeshGPU()
{
	D3DApp::GetInstance()->BeginList();
	ID3D12Device* device = D3DApp::GetInstance()->GetDevice();
	ID3D12GraphicsCommandList* cmdList = D3DApp::GetInstance()->GetCommandList();

	const UINT64 iBufferSize = sizeof(Indices);
	const UINT64 vBufferSize = sizeof(Vertices);

	Name = typeid(D3DMesh).name();

	UINT vertexSize = (UINT)sizeof(XMFLOAT3) + (UINT)sizeof(UINT) + (UINT)sizeof(XMFLOAT2);
	VertexBufferGPU = CreateDefaultBuffer(device, cmdList, Vertices.data(), vBufferSize, VertexBufferUploader);
	VertexByteStride = vertexSize;
	VertexBufferByteSize = vertexSize * Vertices.size();

	IndexBufferGPU = CreateDefaultBuffer(device, cmdList, Indices.data(), iBufferSize, IndexBufferUploader);
	IndexFormat = DXGI_FORMAT_R16_UINT;
	IndexBufferByteSize = iBufferSize;

	IndexCount = Indices.size();
	StartIndexLocation = 0;
	BaseVertexLocation = 0;

	D3DApp::GetInstance()->EndList();
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

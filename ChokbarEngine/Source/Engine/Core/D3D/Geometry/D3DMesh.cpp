#include "Chokbar.h"

#include "D3DMesh.h"
#include "D3D/Base/D3DRenderer.h"

using namespace DirectX;

Vertex::Vertex() : Position(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 0.0f), TangentU(0.0f, 0.0f, 0.0f), TexC(0.0f, 0.0f) { }
Vertex::Vertex(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT4 c, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT3& t, const DirectX::XMFLOAT2& uv) : Position(p), Color(c), Normal(n), TangentU(t), TexC(uv) { }
Vertex::Vertex(
	float px, float py, float pz,
	float cr, float cg, float cb, float ca,
	float nx, float ny, float nz,
	float tx, float ty, float tz,
	float u, float v
) : Position(px, py, pz), Color(cr, cg, cb, ca), Normal(nx, ny, nz), TangentU(tx, ty, tz), TexC(u, v) { }

D3DMesh::D3DMesh()
	: m_vertexByteStride(0), m_vertexBufferByteSize(0), m_indexFormat(DXGI_FORMAT_R16_UINT), m_indexBufferByteSize(0)
{
	m_pVertexBufferGpu = nullptr;
	m_pIndexBufferGpu = nullptr;

	m_pVertexBufferUploader = nullptr;
	m_pIndexBufferUploader = nullptr;
}

D3DMesh::~D3DMesh()
{
	DisposeUploaders();

	RELPTR(m_pVertexBufferGpu);
	RELPTR(m_pIndexBufferGpu);
}

D3D12_VERTEX_BUFFER_VIEW D3DMesh::VertexBufferView() const {
	D3D12_VERTEX_BUFFER_VIEW vbv;
	vbv.BufferLocation = m_pVertexBufferGpu->GetGPUVirtualAddress();
	vbv.StrideInBytes = m_vertexByteStride;
	vbv.SizeInBytes = m_vertexBufferByteSize;

	return vbv;
}

D3D12_INDEX_BUFFER_VIEW D3DMesh::IndexBufferView() const {
	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = m_pIndexBufferGpu->GetGPUVirtualAddress();
	ibv.Format = m_indexFormat;
	ibv.SizeInBytes = m_indexBufferByteSize;

	return ibv;
}

void D3DMesh::DisposeUploaders()
{
	RELPTR(m_pVertexBufferUploader);
	RELPTR(m_pIndexBufferUploader);
}

void D3DMesh::Create(const void* vData, UINT vStride, UINT vSize, const void* iData, UINT iStride, UINT iSize)
{
	I(D3DRenderer)->BeginList();
	ID3D12Device* device = I(D3DRenderer)->GetDevice();
	ID3D12GraphicsCommandList* cmdList = I(D3DRenderer)->GetCommandList();

	const UINT iBufferSize = iStride * iSize;
	const UINT vBufferSize = vStride * vSize;

	Name = typeid(D3DMesh).name();

	m_pVertexBufferGpu = CreateDefaultBuffer(device, cmdList, vData, vBufferSize, m_pVertexBufferUploader);
	m_vertexByteStride = vStride;
	m_vertexBufferByteSize = vBufferSize;

	m_pIndexBufferGpu = CreateDefaultBuffer(device, cmdList, iData, iBufferSize, m_pIndexBufferUploader);
	m_indexFormat = DXGI_FORMAT_R32_UINT;
	m_indexBufferByteSize = iBufferSize;

	m_indexCount = iSize;
	m_startIndexLocation = 0;
	m_baseVertexLocation = 0;

	I(D3DRenderer)->EndList();

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

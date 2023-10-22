#include "Chokbar.h"
#include "MeshGeometry.h"

MeshGeometry::MeshGeometry() : VertexByteStride(0), VertexBufferByteSize(0), IndexFormat(DXGI_FORMAT_R16_UINT), IndexBufferByteSize(0)
{
	VertexBufferCPU = nullptr;
	IndexBufferCPU = nullptr;

	VertexBufferGPU = nullptr;
	IndexBufferGPU = nullptr;

	VertexBufferUploader = nullptr;
	IndexBufferUploader = nullptr;
}

MeshGeometry::~MeshGeometry() 
{
	VertexBufferUploader->Release();
	IndexBufferUploader->Release();

	VertexBufferGPU->Release();
	IndexBufferGPU->Release();

	VertexBufferCPU->Release();
	IndexBufferCPU->Release();
}

D3D12_VERTEX_BUFFER_VIEW MeshGeometry::VertexBufferView() const {
	D3D12_VERTEX_BUFFER_VIEW vbv;
	vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
	vbv.StrideInBytes = VertexByteStride;
	vbv.SizeInBytes = VertexBufferByteSize;

	return vbv;
}

D3D12_INDEX_BUFFER_VIEW MeshGeometry::IndexBufferView() const {
	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
	ibv.Format = IndexFormat;
	ibv.SizeInBytes = IndexBufferByteSize;

	return ibv;
}

void MeshGeometry::DisposeUploaders() 
{
	VertexBufferUploader->Release();
	IndexBufferUploader->Release();
}
#pragma once

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d12.h>
#include <Core/d3dx12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include <wrl.h>
using namespace Microsoft::WRL;

#include <memory>

using namespace DirectX;

template<typename T>
class UploadBuffer
{
public:
	UploadBuffer(ID3D12Device* device, UINT elementCount, BOOL isConstantBuffer);
	~UploadBuffer();

	UploadBuffer(const UploadBuffer& rhs) = delete;
	UploadBuffer& operator=(const UploadBuffer& rhs) = delete;

	UINT GetElementByteSize() const { return m_elementByteSize; }
	ID3D12Resource* GetResource() const { return m_uploadBuffer.Get(); }
	BYTE*& GetMappedData() { return m_mappedData; }

	void CopyData(int elementIndex, const void* data);

private:
	ComPtr<ID3D12Resource> m_uploadBuffer;
	UINT m_elementByteSize;
	BYTE* m_mappedData;
	BOOL m_isConstantBuffer;
};

template<typename T>
UploadBuffer<T>::UploadBuffer(ID3D12Device* device, UINT elementCount, BOOL isConstantBuffer) :
	m_isConstantBuffer(isConstantBuffer), m_mappedData(nullptr), m_elementByteSize(0)
{
	m_elementByteSize = sizeof(T);
	
	if (isConstantBuffer)
		m_elementByteSize = (sizeof(T) + 255) & ~255;

	device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(m_elementByteSize * elementCount),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_uploadBuffer));

	m_uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedData));
}


template<typename T>
UploadBuffer<T>::~UploadBuffer()
{
	if (m_uploadBuffer != nullptr)
		m_uploadBuffer->Unmap(0, nullptr);

	m_mappedData = nullptr;
}

template<typename T>
void UploadBuffer<T>::CopyData(int elementIndex, const void* data)
{
	memcpy(&m_mappedData[elementIndex * m_elementByteSize], data, sizeof(T));
}


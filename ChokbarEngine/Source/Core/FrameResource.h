#pragma once 

struct CHOKBAR_API FrameResource
{
	FrameResource(ID3D12Device* device, UINT passCount, UINT objectCount);
	FrameResource(const FrameResource& rhs) = delete;
	FrameResource& operator=(const FrameResource& rhs) = delete;
	~FrameResource();

	/* Allocator cannot be reset until the GPU is done processing the commands
	Each frame need its own allocator */
	ID3D12CommandAllocator* CmdListAlloc;
	
	UploadBuffer<PassConstants>* PassConstantBuffer;
	UploadBuffer<ObjectConstants>* ObjectConstantBuffer;

	UINT64 Fence;
};
#include "Chokbar.h"
#include "FrameResource.h"

FrameResource::FrameResource(ID3D12Device* device, UINT passCount, UINT objectCount)
{
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&CmdListAlloc));

	ObjectConstantBuffer = new UploadBuffer<ObjectConstants>(device, objectCount, true);
	PassConstantBuffer = new UploadBuffer<PassConstants>(device, passCount, true);

	Fence = 0;
}

FrameResource::~FrameResource()
{
	delete ObjectConstantBuffer;
	delete PassConstantBuffer;
}
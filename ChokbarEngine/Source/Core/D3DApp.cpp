#include "Chokbar.h"
#include "D3DApp.h"
#include <cassert>


D3DApp* D3DApp::m_pApp = nullptr;

D3DApp::D3DApp() :
	m_4xMsaaState(false), m_4xMsaaQuality(0),
	m_RtvDescriptorSize(0), m_DsvDescriptorSize(0), m_CbvSrvUavDescriptorSize(0),
	m_D3dDriverType(D3D_DRIVER_TYPE_HARDWARE), m_BackBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM), m_DepthStencilFormat(DXGI_FORMAT_D24_UNORM_S8_UINT),
	m_CurrentFenceValue(0), m_pInstance(nullptr), m_currBackBuffer(0), m_bufferWidth(DEFAULT_WIDTH), m_bufferHeight(DEFAULT_HEIGHT)
{
	m_pDebugController = nullptr;

	m_pDxgiFactory = nullptr;
	m_pD3dDevice = nullptr;
	m_pFence = nullptr;

	m_pCommandQueue = nullptr;
	m_pCommandAllocator = nullptr;
	m_pCommandList = nullptr;

	m_pRtvHeap = nullptr;
	m_pDsvHeap = nullptr;
	m_pCbvHeap = nullptr;

	m_mainPassCB = nullptr;

	m_pyramidGeometry = nullptr;

	m_pSwapChain = nullptr;
	m_pSwapChainBuffer[0] = nullptr;
	m_pSwapChainBuffer[1] = nullptr;
	m_pDepthStencilBuffer = nullptr;

	m_vsByteCode = nullptr;
	m_psByteCode = nullptr;
	m_rootSignature = nullptr;

	if (m_pApp != nullptr)
	{
		MessageBox(NULL, L"Only one instance of D3DApp can be created.", L"Error", MB_OK);
		return;
	}

	m_pApp = this;

	m_pInstance = HInstance();
}

D3DApp::~D3DApp() {
 	m_pDxgiFactory->Release();
	m_pD3dDevice->Release();

	m_pFence->Release();

	m_pCommandList->Release();
	m_pCommandAllocator->Release();
	m_pCommandQueue->Release();

	m_pRtvHeap->Release();
	m_pDsvHeap->Release();
	m_pCbvHeap->Release();

	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i) 
		m_pSwapChainBuffer[i]->Release();
	m_pSwapChain->Release();

	m_pDepthStencilBuffer->Release();

	for (auto& item : m_OpaqueRenderItems)
		delete item;

	for (auto& item : m_TransparentRenderItems)
		delete item;
	
	for (int i = 0; i < m_mainObjectCB.size(); i++)
		delete m_mainObjectCB[i];
	delete m_mainPassCB;

	m_vsByteCode->Release();
	m_psByteCode->Release();

	m_rootSignature->Release();

	m_pDebugController->Release(); 
}

void D3DApp::Update(const float dt, const float totalTime)
{
	UpdateObjectCB(dt, totalTime);
	UpdateMainPassCB(dt, totalTime);
}

void D3DApp::Render()
{
	// Reset the commandQueue and prepare it for the next frame
	m_pCommandAllocator->Reset();

	m_pCommandList->Reset(m_pCommandAllocator, nullptr);

	// Set resource barrier to transition the back buffer from present to render target
	// This allows to draw to the back buffer (D3D12_RESOURCE_STATE_RENDER_TARGET state)
	// /!\ When the resource barrier is set to D3D12_RESOURCE_STATE_RENDER_TARGET, you back buffer is set to be used as a render target, it cannot be presented.
	CD3DX12_RESOURCE_BARRIER bPresentToTarget = CD3DX12_RESOURCE_BARRIER::Transition(m_pSwapChainBuffer[m_currBackBuffer], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_pCommandList->ResourceBarrier(1, &bPresentToTarget);

	// Create Viewport and ScissorRect for the current back buffer rendering 
	D3D12_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(m_bufferWidth), static_cast<float>(m_bufferHeight), 0.0f, 1.0f };
	D3D12_RECT scissorRect = { 0, 0, m_bufferWidth, m_bufferHeight };
	m_pCommandList->RSSetViewports(1, &viewport);
	m_pCommandList->RSSetScissorRects(1, &scissorRect);

	// Clear the back buffer and depth buffer 
	float color[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CurrentBackBufferView();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = DepthStencilView();
	m_pCommandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
	m_pCommandList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);
	m_pCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Bind the root signature
	m_pCommandList->SetGraphicsRootSignature(m_rootSignature);
	m_pCommandList->SetPipelineState(m_PSOs[PSO_OPAQUE]);

	// Create the descriptor heap for our updated constant buffers
	ID3D12DescriptorHeap* descriptorHeaps[] = { m_pCbvHeap };
	m_pCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	DrawRenderItems(m_pCommandList, m_OpaqueRenderItems);

	// Set resource barrier to transition the back buffer from render target to present
	// This allows to present the back buffer (D3D12_RESOURCE_STATE_PRESENT state)
	// /!\ When the resource barrier is set to D3D12_RESOURCE_STATE_PRESENT, you cannot draw to the back buffer anymore
	CD3DX12_RESOURCE_BARRIER bTargetToPresent = CD3DX12_RESOURCE_BARRIER::Transition(m_pSwapChainBuffer[m_currBackBuffer], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_pCommandList->ResourceBarrier(1, &bTargetToPresent);

	// Close the command list (mandatory before calling ExecuteCommandLists)
	m_pCommandList->Close();

	// Execute the command list and flush the command queue
	// Refer to the m_pFence member to understand how the command queue is flushed
	ID3D12CommandList* cmdLists[] = { m_pCommandList };
	m_pCommandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);

	// Present the back buffer to the screen and swap the front/back buffer
	m_pSwapChain->Present(0, 0);
	m_currBackBuffer = (m_currBackBuffer + 1) % SWAP_CHAIN_BUFFER_COUNT;

	FlushCommandQueue();
}

void D3DApp::OnResize(SIZE windowSize)
{
	m_bufferWidth = windowSize.cx;
	m_bufferHeight = windowSize.cy;
}

D3DApp* D3DApp::GetInstance()
{
	if (m_pApp == nullptr)
	{
		m_pApp = new D3DApp();
	}

	return m_pApp;
}

void D3DApp::InitializeD3D12(Win32::Window* window)
{
#if defined(DEBUG) || defined(_DEBUG)
	EnableDebugLayer();
#endif

	m_bufferWidth = window->Size().cx;
	m_bufferHeight = window->Size().cy;

	CreateDevice();
	CreateFenceAndGetDescriptorsSizes();
	CheckMSAAQualitySupport();
	CreateCommandObjects();
	CreateSwapChain(window->Handle());

	CreateRtvAndDsvDescriptorHeaps();
	CreateRenderTargetView();

	CreateGeometry();
	CreateRenderItems();

	RegisterInitCommands_In_CommandList();

	CreatePipelineStateObject();
}

void D3DApp::EnableDebugLayer()
{
	// Enable the D3D12 debug layer.
	D3D12GetDebugInterface(IID_PPV_ARGS(&m_pDebugController));
	m_pDebugController->EnableDebugLayer();
}

void D3DApp::CreateDevice()
{
	CreateDXGIFactory1(IID_PPV_ARGS(&m_pDxgiFactory));

	D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pD3dDevice));

#if defined(DEBUG) || defined(_DEBUG)
	// DEBUG_CreateInfoQueue();
#endif
}

void D3DApp::CreateFenceAndGetDescriptorsSizes()
{
	m_pD3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence));

	m_RtvDescriptorSize = m_pD3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_DsvDescriptorSize = m_pD3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_CbvSrvUavDescriptorSize = m_pD3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void D3DApp::CheckMSAAQualitySupport()
{
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = m_BackBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;

	m_pD3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels));

	m_4xMsaaQuality = msQualityLevels.NumQualityLevels;
	m_4xMsaaState = SUCCEEDED(m_pD3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels)));
	assert(m_4xMsaaQuality > 0 && "Unexpected MSAA quality level.");
}

void D3DApp::DEBUG_CreateInfoQueue()
{
	ID3D12InfoQueue* InfoQueue = nullptr;
	m_pD3dDevice->QueryInterface(IID_PPV_ARGS(&InfoQueue));
	InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
	InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
	InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, false);
}

void D3DApp::CreateCommandObjects()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	m_pD3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pCommandQueue));

	m_pD3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocator));

	m_pD3dDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_pCommandAllocator, // Associated command allocator
		nullptr, // Initial PipelineStateObject
		IID_PPV_ARGS(&m_pCommandList)
	);

	// Start off in a closed state. This is because the first time we
	// refer to the command list we will Reset it, and it needs to be
	// closed before calling Reset.
	m_pCommandList->Close();
}

void D3DApp::CreateSwapChain(HWND windowHandle)
{
	// Release the previous swapchain we will be recreating.
	if (m_pSwapChain != nullptr) m_pSwapChain->Release();

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = m_bufferWidth;
	sd.BufferDesc.Height = m_bufferHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = m_BackBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT;
	sd.OutputWindow = windowHandle;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
	m_pDxgiFactory->CreateSwapChain(m_pCommandQueue, &sd, &m_pSwapChain);
}

void D3DApp::FlushCommandQueue()
{
	// Advance the fence value to mark commands up to this fence point.
	m_CurrentFenceValue++;

	// Add an instruction to the command queue to set a new fence point. 
	// Because we are on the GPU timeline, the new fence point won't be 
	// set until the GPU finishes processing all the commands prior to this Signal().
	m_pCommandQueue->Signal(m_pFence, m_CurrentFenceValue);

	// Wait until the GPU has completed commands up to this fence point.
	if (m_pFence->GetCompletedValue() < m_CurrentFenceValue)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

		// Fire event when GPU hits current fence.  
		m_pFence->SetEventOnCompletion(m_CurrentFenceValue, eventHandle);

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}

void D3DApp::RegisterInitCommands_In_CommandList()
{
	m_pCommandAllocator->Reset();
	m_pCommandList->Reset(m_pCommandAllocator, nullptr);

	CreateDepthStencilBuffer();
	CreateConstantBuffers();

	CreateRootSignature();

	HRESULT hr = m_pCommandList->Close();

	ID3D12CommandList* cmdLists[] = { m_pCommandList };
	m_pCommandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
	FlushCommandQueue();
}

void D3DApp::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;

	m_pD3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_pRtvHeap));


	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;

	m_pD3dDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_pDsvHeap));

	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.NumDescriptors = m_ObjectCount + 1;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;

	m_pD3dDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_pCbvHeap));

}

void D3DApp::CreateDepthStencilBuffer()
{
	// Create the depth/stencil buffer and view.
	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = m_bufferWidth;
	depthStencilDesc.Height = m_bufferHeight;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = m_DepthStencilFormat;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = m_DepthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;

	m_pD3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(&m_pDepthStencilBuffer)
	);

	// Create descriptor to mip level 0 of entire resource using the
	// format of the resource.
	m_pD3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, DepthStencilView());

	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_pDepthStencilBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	m_pCommandList->ResourceBarrier(1, &barrier);
}

void D3DApp::CreateRenderTargetView()
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_pRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		// Get the ith buffer in the swap chain.
		m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pSwapChainBuffer[i]));

		// Create an RTV to it.
		m_pD3dDevice->CreateRenderTargetView(m_pSwapChainBuffer[i], nullptr, rtvHeapHandle);

		// Next entry in heap.
		rtvHeapHandle.Offset(1, m_RtvDescriptorSize);
	}
}

void D3DApp::CreateGeometry()
{
	Vertex vList[] =
	{
		{ XMFLOAT3(0.0f, 1.0f, 0.0f),   0xFF00FF00 },
		{ XMFLOAT3(-0.5f, 0.0f, -0.5f), 0xFF000000 },
		{ XMFLOAT3(-0.5f, 0.0f, 0.5f),  0xFF000000 },
		{ XMFLOAT3(0.5f, 0.0f, 0.5f),   0xFF000000 },
		{ XMFLOAT3(0.5f, 0.0f, -0.5f),  0xFF000000 },
	};

	m_inputLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	m_inputLayout[1] = { "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, sizeof(XMFLOAT3), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	UINT16 iList[] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1 };

	m_pCommandAllocator->Reset();
	m_pCommandList->Reset(m_pCommandAllocator, nullptr);

	const UINT64 iBufferSize = sizeof(iList);
	const UINT64 vBufferSize = sizeof(vList);

	m_pyramidGeometry = new MeshGeometry();
	m_pyramidGeometry->Name = "Pyramid";

	m_pyramidGeometry->VertexBufferGPU = CreateDefaultBuffer(vList, vBufferSize, m_pyramidGeometry->VertexBufferUploader);
	m_pyramidGeometry->VertexByteStride = sizeof(Vertex);
	m_pyramidGeometry->VertexBufferByteSize = sizeof(Vertex) * _countof(vList);

	m_pyramidGeometry->IndexBufferGPU = CreateDefaultBuffer(iList, iBufferSize, m_pyramidGeometry->IndexBufferUploader);
	m_pyramidGeometry->IndexFormat = DXGI_FORMAT_R16_UINT;
	m_pyramidGeometry->IndexBufferByteSize = iBufferSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = _countof(iList);
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;
	m_pyramidGeometry->DrawArgs["Pyramid"] = submesh;

	HRESULT hr = m_pCommandList->Close();

	ID3D12CommandList* cmdLists[] = { m_pCommandList };
	m_pCommandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
	FlushCommandQueue();
}

void D3DApp::CreateRenderItems()
{
	for (int i = 0; i < m_ObjectCount; i++)
	{
		auto pyrItem = RenderItem();
		pyrItem.ObjCBIndex = m_AllRenderItems.size();
		pyrItem.Geo = m_pyramidGeometry;
		pyrItem.PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		pyrItem.IndexCount = pyrItem.Geo->DrawArgs["Pyramid"].IndexCount;
		pyrItem.StartIndexLocation = pyrItem.Geo->DrawArgs["Pyramid"].StartIndexLocation;
		pyrItem.BaseVertexLocation = pyrItem.Geo->DrawArgs["Pyramid"].BaseVertexLocation;
		pyrItem.Transform = Transform();
		if (i == 0) 
		{
			pyrItem.TransformationType = TRANSFORMATION_TYPE::ROTATION;
			pyrItem.Transform.Position = XMFLOAT3(1.0f, 0.0f, 0.0f);
		}
		else 
		{
			pyrItem.TransformationType = TRANSFORMATION_TYPE::TRANSLATION;
			pyrItem.Transform.Position = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		}
		XMStoreFloat4x4(&pyrItem.World, XMMatrixTranspose(XMMatrixTranslationFromVector(XMLoadFloat3(&pyrItem.Transform.Position))));
			
		m_AllRenderItems.push_back(std::move(pyrItem));
	}

	for (auto& ri : m_AllRenderItems)
	{
		m_OpaqueRenderItems.push_back(&ri);
	}
}

void D3DApp::CreateConstantBuffers()
{
	const UINT64 cBufferSize = (sizeof(ObjectConstants) + 255) & ~255;
	const UINT numCB = 1;

	m_mainPassCB = new UploadBuffer<PassConstants>(m_pD3dDevice, 1, true);
	D3D12_GPU_VIRTUAL_ADDRESS passCBAddress = m_mainPassCB->GetResource()->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC passCBVDesc;
	passCBVDesc.BufferLocation = passCBAddress;
	passCBVDesc.SizeInBytes = (UINT)cBufferSize;
	m_pD3dDevice->CreateConstantBufferView(&passCBVDesc, m_pCbvHeap->GetCPUDescriptorHandleForHeapStart());

	for (int i = 0; i < m_AllRenderItems.size(); i++)
	{
		m_mainObjectCB.push_back(new UploadBuffer<ObjectConstants>(m_pD3dDevice, numCB, true));
		D3D12_GPU_VIRTUAL_ADDRESS cbAddress = m_mainObjectCB[i]->GetResource()->GetGPUVirtualAddress();

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
		cbvDesc.BufferLocation = cbAddress;
		cbvDesc.SizeInBytes = (UINT)cBufferSize;

		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvHeapHandle(m_pCbvHeap->GetCPUDescriptorHandleForHeapStart());
		cbvHeapHandle.Offset(i + 1, m_CbvSrvUavDescriptorSize);

		m_pD3dDevice->CreateConstantBufferView(&cbvDesc, cbvHeapHandle);
	}

	UpdateObjectCB(0, 0);
	UpdateMainPassCB(0, 0);

	// Offset to the ith object constant buffer in the buffer here.

}

void D3DApp::UpdateObjectCB(const float dt, const float totalTime)
{

	for (auto& item : m_AllRenderItems)
	{
		auto assiociatedCB = m_mainObjectCB[item.ObjCBIndex];
		XMMATRIX world = XMLoadFloat4x4(&item.World);

		switch (item.TransformationType)
		{
		case TRANSFORMATION_TYPE::ROTATION:
		{
			item.Transform.Rotation.y += 1.0f * dt;
			world = XMMatrixRotationY(item.Transform.Rotation.y);
			break;
		}
		case TRANSFORMATION_TYPE::TRANSLATION:
		{
			item.Transform.Position.y = sinf(totalTime) * 2.0f;
			world = XMMatrixTranslationFromVector(XMLoadFloat3(&item.Transform.Position));
			break;
		}
		default:
			break;
		}

		XMStoreFloat4x4(&item.World, XMMatrixTranspose(world));

		ObjectConstants objConstants;
		XMStoreFloat4x4(&objConstants.World, XMLoadFloat4x4(&item.World));
		assiociatedCB->CopyData(0, &objConstants);
	}
}

void D3DApp::UpdateMainPassCB(const float dt, const float totalTime)
{
	m_camera.UpdateProjMatrix(m_bufferWidth, m_bufferHeight);
	m_camera.UpdateViewMatrix();

	XMMATRIX camView = XMLoadFloat4x4(&m_camera.View);
	XMMATRIX camProj = XMLoadFloat4x4(&m_camera.Proj);

	XMMATRIX viewProj		= XMMatrixMultiply(camView, camProj);
	XMMATRIX invView		= XMMatrixInverse(&XMMatrixDeterminant(camView), camView);
	XMMATRIX invProj		= XMMatrixInverse(&XMMatrixDeterminant(camProj), camProj);
	XMMATRIX invViewProj	= XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);

	PassConstants mainPassCB;
	XMStoreFloat4x4(&mainPassCB.View,			XMMatrixTranspose(camView));
	XMStoreFloat4x4(&mainPassCB.InvView,		XMMatrixTranspose(invView));
	XMStoreFloat4x4(&mainPassCB.Proj,			XMMatrixTranspose(camProj));
	XMStoreFloat4x4(&mainPassCB.InvProj,		XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&mainPassCB.ViewProj,		XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&mainPassCB.InvViewProj,	XMMatrixTranspose(invViewProj));

	mainPassCB.EyePosW = m_camera.Position;
	mainPassCB.RenderTargetSize = XMFLOAT2(m_bufferWidth, m_bufferHeight);
	mainPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / m_bufferWidth, 1.0f / m_bufferHeight);
	mainPassCB.NearZ = m_camera.NearZ;
	mainPassCB.FarZ = m_camera.FarZ;
	mainPassCB.TotalTime = totalTime;
	mainPassCB.DeltaTime = dt;

	m_mainPassCB->CopyData(0, &mainPassCB);
}

void D3DApp::CreateObject()
{

}

void D3DApp::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& renderItems)
{
	cmdList->SetGraphicsRootConstantBufferView(1, m_mainPassCB->GetResource()->GetGPUVirtualAddress());

	const UINT64 cBufferSize = (sizeof(ObjectConstants) + 255) & ~255;
	for (int i = 0; i < renderItems.size(); i++)
	{
		auto renderItem = renderItems[i];

		cmdList->IASetVertexBuffers(0, 1, &renderItem->Geo->VertexBufferView());
		cmdList->IASetIndexBuffer(&renderItem->Geo->IndexBufferView());
		cmdList->IASetPrimitiveTopology(renderItem->PrimitiveType);

		UINT cbvIndex = renderItem->ObjCBIndex;
		auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_pCbvHeap->GetGPUDescriptorHandleForHeapStart());
		cbvHandle.Offset(cbvIndex, m_CbvSrvUavDescriptorSize);

		cmdList->SetGraphicsRootConstantBufferView(0, m_mainObjectCB[i]->GetResource()->GetGPUVirtualAddress());

		cmdList->DrawIndexedInstanced(renderItem->IndexCount, 1, renderItem->StartIndexLocation, renderItem->BaseVertexLocation, 0);
	}
}

void D3DApp::CreateRootSignature()
{
	//CD3DX12_DESCRIPTOR_RANGE cbvTable0;
	//cbvTable0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

	//CD3DX12_DESCRIPTOR_RANGE cbvTable1;
	//cbvTable1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);

	CD3DX12_ROOT_PARAMETER slotRootParameter[2];
	slotRootParameter[0].InitAsConstantBufferView(0);
	slotRootParameter[1].InitAsConstantBufferView(1);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(_countof(slotRootParameter), slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* serializedRootSignature = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSignature, &errorBlob);
	m_pD3dDevice->CreateRootSignature(0, serializedRootSignature->GetBufferPointer(), serializedRootSignature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));

	if (errorBlob != nullptr) errorBlob->Release();

	m_pCommandList->SetGraphicsRootSignature(m_rootSignature);
	serializedRootSignature->Release();
}

void D3DApp::CreatePipelineStateObject()
{
	m_vsByteCode = CompileShader(L"Shader/vertex_pixel.hlsl", nullptr, "vs_main", "vs_5_0");
	m_psByteCode = CompileShader(L"Shader/vertex_pixel.hlsl", nullptr, "ps_main", "ps_5_0");

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { m_inputLayout, 2 };
	psoDesc.pRootSignature = m_rootSignature;
	psoDesc.VS = { reinterpret_cast<BYTE*>(m_vsByteCode->GetBufferPointer()), m_vsByteCode->GetBufferSize() };
	psoDesc.PS = { reinterpret_cast<BYTE*>(m_psByteCode->GetBufferPointer()), m_psByteCode->GetBufferSize() };
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = m_BackBufferFormat;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;
	psoDesc.DSVFormat = m_DepthStencilFormat;

	m_pD3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PSOs[PSO_OPAQUE]));
}

ID3D12Resource* D3DApp::CreateDefaultBuffer(const void* initData, UINT64 byteSize, ID3D12Resource* uploadBuffer)
{
	ID3D12Resource* defaultBuffer = nullptr;

	CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
	m_pD3dDevice->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&defaultBuffer));
	if (defaultBuffer == nullptr) return nullptr;

	CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	m_pD3dDevice->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadBuffer));
	if (uploadBuffer == nullptr) return nullptr;

	D3D12_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pData = initData;
	subresourceData.RowPitch = byteSize;
	subresourceData.SlicePitch = subresourceData.RowPitch;

	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
	m_pCommandList->ResourceBarrier(1, &barrier);
	UpdateSubresources<1>(m_pCommandList, defaultBuffer, uploadBuffer, 0, 0, 1, &subresourceData);
	m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

	return defaultBuffer;
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DApp::CurrentBackBufferView() const
{

	// CD3DX12 constructor to offset to the RTV of the current back buffer.
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(),		// handle start
		m_currBackBuffer,										// index to offset
		m_RtvDescriptorSize										// byte size of descriptor
	);
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DApp::DepthStencilView() const
{
	return m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();
}

ID3DBlob* D3DApp::CompileShader(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target)
{
	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = S_OK;

	ID3DBlob* byteCode = nullptr;
	ID3DBlob* errors = nullptr;

	hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

	if (errors != nullptr)
	{
		std::cout << ((char*)errors->GetBufferPointer()) << std::endl;
		errors->Release();
	}
	ThrowIfFailed(hr);

	return byteCode;
}
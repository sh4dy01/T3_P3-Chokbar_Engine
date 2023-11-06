#include "Chokbar.h"

#include "Core/DebugUtils.h"

#include "Engine/Resource.h"

#include "D3DMath.h"

#include "Engine/ECS/Components/TransformComponent.h"
#include "Core/D3D/Internal/Texture.h"
#include "Core/D3D/Internal/ShaderBase.h"
#include "Core/D3D/Internal/MeshRenderer.h"
#include "Core/D3D/Internal/Material.h"
#include "Engine/Engine.h"
#include "D3DApp.h"

using namespace DirectX;

#pragma region BASIC
D3DApp* D3DApp::m_pApp = nullptr;

D3DApp::D3DApp() :
	m_pInstance(nullptr), m_4xMsaaState(false),
	m_4xMsaaQuality(0), m_bufferWidth(DEFAULT_WIDTH), m_bufferHeight(DEFAULT_HEIGHT),
	m_D3dDriverType(D3D_DRIVER_TYPE_HARDWARE), m_CurrentFenceValue(0), m_RtvDescriptorSize(0),
	m_DsvDescriptorSize(0), m_CbvSrvUavDescriptorSize(0), m_currBackBuffer(0), m_BackBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM), m_DepthStencilFormat(DXGI_FORMAT_D24_UNORM_S8_UINT),
	m_IsVsyncEnabled(true), m_IsFrustumCullingEnabled(true), m_texIndex(0)
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

	m_pSwapChain = nullptr;
	m_pSwapChainBuffer[0] = nullptr;
	m_pSwapChainBuffer[1] = nullptr;
	m_pDepthStencilBuffer = nullptr;

	if (m_pApp != nullptr)
	{
		MessageBox(NULL, L"Only one instance of D3DApp can be created.", L"Error", MB_OK);
		return;
	}

	m_pApp = this;

	m_pInstance = HInstance();
}

D3DApp::~D3DApp() {
	RELPTR(m_pDxgiFactory);
	RELPTR(m_pD3dDevice);

	RELPTR(m_pFence);

	RELPTR(m_pCommandList);
	RELPTR(m_pCommandAllocator);
	RELPTR(m_pCommandQueue);

	RELPTR(m_pRtvHeap);
	RELPTR(m_pDsvHeap);
	RELPTR(m_pCbvHeap);

	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
		RELPTR(m_pSwapChainBuffer[i]);
	RELPTR(m_pSwapChain);

	RELPTR(m_pDepthStencilBuffer);

	RELPTR(m_pDebugController);
}

void D3DApp::Update(const float dt, const float totalTime)
{
	Engine::GetMainCamera()->UpdateViewMatrix();

	UpdateRenderItems(dt, totalTime);

	for (auto& shader : Resource::GetShaders())
	{
		shader.second->UpdatePassCB(dt, totalTime);
	}
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
	m_pCommandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);
	m_pCommandList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);
	m_pCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Create the descriptor heap for our updated constant buffers
	ID3D12DescriptorHeap* descriptorHeaps[] = { m_pCbvHeap };
	m_pCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	DrawRenderItems(m_pCommandList);

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

	m_pSwapChain->Present(m_IsVsyncEnabled ? 1 : 0, 0);

	m_currBackBuffer = (m_currBackBuffer + 1) % SWAP_CHAIN_BUFFER_COUNT;

	FlushCommandQueue();
}

void D3DApp::OnResize(int newWidth, int newHeight)
{
	m_bufferWidth = newWidth;
	m_bufferHeight = newHeight;

	BoundingFrustum::CreateFromMatrix(m_Frustum, Engine::GetMainCamera()->GetProj());
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

	m_bufferWidth = window->GetWidth();
	m_bufferHeight = window->GetHeight();

	CreateDevice();
	CreateFenceAndGetDescriptorsSizes();
	CheckMSAAQualitySupport();
	CreateCommandObjects();
	CreateSwapChain(window->GetHandle());

	CreateRtvAndDsvDescriptorHeaps();
	CreateRenderTargetView();
	CreateDepthStencilBuffer();

	GeometryHandler::CreateAllMeshes();

	CreateResources();
	GetMeshRenderersRef();

}
#pragma endregion

#pragma region COMMAND_LIST_PUBLIC
void D3DApp::BeginList()
{
	m_pCommandAllocator->Reset();
	m_pCommandList->Reset(m_pCommandAllocator, nullptr);
}

void D3DApp::EndList()
{
	HRESULT hr = m_pCommandList->Close();

	ID3D12CommandList* cmdLists[] = { m_pCommandList };
	m_pCommandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
	FlushCommandQueue();
}
#pragma endregion

#pragma region D3DX12_COMPONENTS
void D3DApp::EnableDebugLayer()
{
	// Enable the D3D12 debug layer.
	D3D12GetDebugInterface(IID_PPV_ARGS(&m_pDebugController));
	//m_pDebugController->EnableDebugLayer();
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
	cbvHeapDesc.NumDescriptors = 10;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;

	m_pD3dDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_pCbvHeap));

}

void D3DApp::CreateDepthStencilBuffer()
{
	BeginList();

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

	EndList();
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
#pragma endregion

#pragma region COMMAND_LIST_PRIVATE
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
#pragma endregion

#pragma region CREATE_INTERNAL_COMPONENTS

void D3DApp::CreateResources()
{
	Resource::CreateResources(m_pD3dDevice, m_pCbvHeap, m_CbvSrvUavDescriptorSize);

	auto& shaders = Resource::GetShaders();
	shaders[SIMPLE]->CreatePsoAndRootSignature(VertexType::POS_COLOR, m_BackBufferFormat, m_DepthStencilFormat);
	shaders[TEXTURE]->CreatePsoAndRootSignature(VertexType::POS_NORM_TAN_TEX, m_BackBufferFormat, m_DepthStencilFormat);
}

void D3DApp::GetMeshRenderersRef()
{
	m_meshRenderers = Engine::GetCoordinator()->GetAllComponentsOfType<MeshRenderer>()->GetAllData();
}
#pragma endregion

#pragma region UPDATE 
int D3DApp::UpdateTextureHeap(Texture* tex)
{
	if (!tex) return -1;

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(m_pCbvHeap->GetCPUDescriptorHandleForHeapStart());
	hDescriptor.Offset(m_texIndex, m_CbvSrvUavDescriptorSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = tex->Resource->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = tex->Resource->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	m_pD3dDevice->CreateShaderResourceView(tex->Resource, &srvDesc, hDescriptor);

	return m_texIndex++;
}

void D3DApp::UpdateRenderItems(const float dt, const float totalTime)
{
	BoundingFrustum::CreateFromMatrix(m_Frustum, Engine::GetMainCamera()->GetProj());
	m_Frustum.Origin = Engine::GetMainCamera()->transform->GetPosition();
	m_Frustum.Orientation = Engine::GetMainCamera()->transform->GetQuaternion();
	//m_Frustum.Origin = Engine::GetMainCamera()->transform->GetPosition();
	//m_Frustum.Orientation = Engine::GetMainCamera()->transform->GetQuaternion();


	const XMMATRIX view = CameraManager::GetMainCamera()->GetView();
	const XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);

	int visibleMeshRendered = 0;
	for (const auto mr : *m_meshRenderers)
	{
		if (!mr || !mr->IsEnabled()) continue;

		mr->transform->UpdateWorldMatrix();

		//mr->Bounds.Center = mr->transform->GetPosition();
		//mr->Bounds.Radius = mr->transform->GetMaxScale();

		if (m_IsFrustumCullingEnabled && IsObjectInFrustum(invView, mr))
		{
			visibleMeshRendered++;

			mr->Mat->GetShader()->UpdateObjectCB(mr->transform->GetWorldMatrix(), mr->ObjectCBIndex);
		}
	}

	DEBUG_LOG("Visible Mesh Updated: " << visibleMeshRendered)
}

void D3DApp::DrawRenderItems(ID3D12GraphicsCommandList* cmdList)
{
	const XMMATRIX view = CameraManager::GetMainCamera()->GetView();
	const XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);

	int visibleMeshRendered = 0;
	for (const auto mr : *m_meshRenderers)
	{
		if (!mr || !mr->IsEnabled()) continue;

		//mr->Bounds.Center = mr->transform->GetPosition();
		//mr->Bounds.Radius = mr->transform->GetMaxScale();

		if (m_IsFrustumCullingEnabled && IsObjectInFrustum(invView, mr))
		{
			visibleMeshRendered++;

			mr->Mat->GetShader()->BeginDraw(cmdList);

			ShaderDrawArguments args;

			args.CmdList = cmdList;

			args.ItemGeometry = mr->Mesh;
			args.ItemCBIndex = mr->ObjectCBIndex;
			args.IndexCount = mr->Mesh->GetIndexCount();
			args.StartIndexLocation = 0;
			args.BaseVertexLocation = 0;

			args.Text = mr->GetTextures().empty() ? nullptr : mr->GetTexture(0);
			args.TextSrvIndex = mr->TextSrvIndex;

			mr->Mat->GetShader()->Draw(args);

			mr->Mat->GetShader()->EndDraw(cmdList);
		}
	}

	DEBUG_LOG("Visible Mesh Rendered: " << visibleMeshRendered)
}

bool D3DApp::IsObjectInFrustum(const XMMATRIX& invView, const MeshRenderer* const mr)
{
	const XMMATRIX world = XMLoadFloat4x4(mr->transform->GetWorldMatrix());
	//XMMATRIX texTransform = XMLoadFloat4x4(&instanceData[i].TexTransform);

	const XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(world), world);

	// View space to the object's local space.
	const XMMATRIX viewToLocal = XMMatrixMultiply(invView, invWorld);

	// Get the local space bounding frustum of the camera in view space.
	BoundingFrustum localSpaceFrustum;
	m_Frustum.Transform(localSpaceFrustum, viewToLocal);

	// Perform the sphere/frustum intersection test in local space.
	return localSpaceFrustum.Contains(mr->Bounds) != DirectX::DISJOINT;
}

#pragma endregion

#pragma region CONSTANTS
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
#pragma endregion

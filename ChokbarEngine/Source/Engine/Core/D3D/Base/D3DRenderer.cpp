#include "Chokbar.h"

#include "Engine/Resource.h"

#include "D3D/Shaders/Texture.h"
#include "D3D/Shaders/ShaderBase.h"

#include "D3D/Renderers/MeshRenderer.h"
#include "D3D/Renderers/ParticleRenderer.h"
#include "D3D/Renderers/SkyRenderer.h"
#include "D3D/Renderers/UIRenderer.h"

#include "D3D/Geometry/GeometryHandler.h"

#include "Engine/Engine.h"

#include "D3DRenderer.h"

#include <ranges>

using namespace DirectX;

#pragma region BASIC
D3DRenderer* D3DRenderer::m_pApp = nullptr;

D3DRenderer::D3DRenderer() :
	m_bufferWidth(DEFAULT_WIDTH), m_bufferHeight(DEFAULT_HEIGHT),
	m_pInstance(nullptr), m_4xMsaaState(false), m_4xMsaaQuality(0),
	m_driveType(D3D_DRIVER_TYPE_HARDWARE), m_CurrentFenceValue(0), m_rtvDescriptorSize(0),
	m_dsvDescriptorSize(0), m_cbvSrvUavDescriptorSize(0), m_currBackBuffer(0), m_backBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM), m_depthStencilFormat(DXGI_FORMAT_D24_UNORM_S8_UINT),
	m_meshRenderers(nullptr), m_particleRenderers(nullptr), m_skyRenderers(nullptr), m_texIndex(0)
{
	m_pDebugController = nullptr;

	m_pDxgiFactory = nullptr;
	m_pDevice = nullptr;
	m_pFence = nullptr;

	m_pCommandQueue = nullptr;
	m_pCommandAllocator = nullptr;
	m_pCommandList = nullptr;

	m_pRtvHeap = nullptr;
	m_pDsvHeap = nullptr;
	m_pCbvSrvHeap = nullptr;

	m_pSwapChain = nullptr;
	m_pSwapChainBuffer[0] = nullptr;
	m_pSwapChainBuffer[1] = nullptr;
	m_pDepthStencilBuffer = nullptr;

	if (m_pApp != nullptr)
	{
		MessageBox(nullptr, L"Only one instance of D3DRenderer can be created.", L"Error", MB_OK);
		return;
	}

	m_pApp = this;

	m_pInstance = HInstance();
}

D3DRenderer::~D3DRenderer() {
	/*
	NOTE : DX12 objects are released in the reverse order of their creation
	I.e : We first delete the command list, then the command allocator, then the command queue, etc.
	We cannot store DX12 components ourselves because they are created by the API. An object will be active as long as its reference count is greater than 0.
	By calling the Release() method, we decrease the reference count by 1. When the reference count reaches 0, the object is destroyed.
	*/
	RELPTR(m_pDxgiFactory)
	RELPTR(m_pDevice)

	RELPTR(m_pFence)

	RELPTR(m_pCommandList)
	RELPTR(m_pCommandAllocator)
	RELPTR(m_pCommandQueue)

	RELPTR(m_pRtvHeap)
	RELPTR(m_pDsvHeap)
	RELPTR(m_pCbvSrvHeap)

	for (auto& i : m_pSwapChainBuffer)
		RELPTR(i)
	
	RELPTR(m_pSwapChain)

	RELPTR(m_pDepthStencilBuffer)

	RELPTR(m_pDebugController)

	NULLPTR(m_meshRenderers)
	NULLPTR(m_particleRenderers)

	// Delete all static meshes from GeometryHandler
	GeometryHandler::DestroyAllMeshes();

	// Delete all resources
	Resource::ReleaseResources();
}

void D3DRenderer::Update(const float dt, const float totalTime) const
{
	UpdateRenderedObjects(dt, totalTime);

	Engine::GetMainCamera()->UpdateViewMatrix();

	for (const auto& shader : Resource::GetShaders() | std::views::values)
	{
		shader->UpdatePassCB(dt, totalTime);
	}
}

void D3DRenderer::Render()
{
	// Reset the commandQueue and prepare it for the next frame
	BeginList();

	// Set resource barrier to transition the back buffer from present to render target
	// This allows to draw to the back buffer (D3D12_RESOURCE_STATE_RENDER_TARGET state)
	// /!\ When the resource barrier is set to D3D12_RESOURCE_STATE_RENDER_TARGET, you back buffer is set to be used as a render target, it cannot be presented.
	const CD3DX12_RESOURCE_BARRIER bPresentToTarget = CD3DX12_RESOURCE_BARRIER::Transition(m_pSwapChainBuffer[m_currBackBuffer], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_pCommandList->ResourceBarrier(1, &bPresentToTarget);

	// Create Viewport and ScissorRect for the current back buffer rendering 
	const D3D12_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(m_bufferWidth), static_cast<float>(m_bufferHeight), 0.0f, 1.0f };
	const D3D12_RECT scissorRect = { 0, 0, m_bufferWidth, m_bufferHeight };
	m_pCommandList->RSSetViewports(1, &viewport);
	m_pCommandList->RSSetScissorRects(1, &scissorRect);
	
	// Clear the back buffer and depth buffer 
	constexpr float color[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	const D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CurrentBackBufferView();
	const D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = DepthStencilView();
	m_pCommandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);
	m_pCommandList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);
	m_pCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Create the descriptor heap for our updated constant buffers
	ID3D12DescriptorHeap* descriptorHeaps[] = { m_pCbvSrvHeap };
	m_pCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	RenderObjects();

	// Set resource barrier to transition the back buffer from render target to present
	// This allows to present the back buffer (D3D12_RESOURCE_STATE_PRESENT state)
	// /!\ When the resource barrier is set to D3D12_RESOURCE_STATE_PRESENT, you cannot draw to the back buffer anymore
	const CD3DX12_RESOURCE_BARRIER bTargetToPresent = CD3DX12_RESOURCE_BARRIER::Transition(m_pSwapChainBuffer[m_currBackBuffer], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_pCommandList->ResourceBarrier(1, &bTargetToPresent);

	EndList();

	// Present the back buffer to the screen and swap the front/back buffer
	// DX12 methods often return an Hardware Result (HRESULT) to indicate if the method succeeded or not
	// Our utility function ThrowIfFailed will throw an exception if the HRESULT is not S_OK
	const HRESULT hr = m_pSwapChain->Present(1, 0);
	ThrowIfFailed(hr)
	m_currBackBuffer = (m_currBackBuffer + 1) % SWAP_CHAIN_BUFFER_COUNT;
}

void D3DRenderer::OnResize(const int newWidth, const int newHeight)
{
	m_bufferWidth = newWidth;
	m_bufferHeight = newHeight;

	//TODO : Resize the swap chain and recreate the render target view

	const auto cam = Engine::GetMainCamera();
	//cam->SetLens(0.25f * XM_PI, m_bufferWidth / m_bufferHeight, 1.0f, 1000.0f);

	BoundingFrustum::CreateFromMatrix(m_frustum, cam->GetProj());
}

D3DRenderer* D3DRenderer::GetInstance()
{
	if (m_pApp == nullptr)
	{
		m_pApp = new D3DRenderer();
	}

	return m_pApp;
}

void D3DRenderer::InitializeD3D12(const Win32::Window* window)
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
	GetRenderComponentsRef();
}
#pragma endregion

#pragma region COMMAND_LIST_PUBLIC
void D3DRenderer::BeginList() const
{
	HRESULT hr = m_pCommandAllocator->Reset();
	ThrowIfFailed(hr)
	hr = m_pCommandList->Reset(m_pCommandAllocator, nullptr);
	ThrowIfFailed(hr)
}

void D3DRenderer::EndList()
{
	// Close the command list (mandatory before calling ExecuteCommandLists)
	const HRESULT hr = m_pCommandList->Close();
	ThrowIfFailed(hr)

	// Execute the command list and flush the command queue
	// Refer to the m_pFence member to understand how the command queue is flushed
	ID3D12CommandList* cmdLists[] = { m_pCommandList };
	m_pCommandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
	FlushCommandQueue();
}
#pragma endregion

#pragma region D3DX12_COMPONENTS
void D3DRenderer::EnableDebugLayer()
{
	// Enable the D3D12 debug layer.
	D3D12GetDebugInterface(IID_PPV_ARGS(&m_pDebugController));
	//m_pDebugController->EnableDebugLayer();
}

void D3DRenderer::CreateDevice()
{
	HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&m_pDxgiFactory));
	ThrowIfFailed(hr)

	// Create device with a default adapter
	hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice));

	// If failed, try to create device with WARP adapter (software rasterizer)
	if (FAILED(hr))
	{
		IDXGIAdapter* pWarpAdapter;
		
		hr = m_pDxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter));
		ThrowIfFailed(hr)

		hr = D3D12CreateDevice(pWarpAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice));
		ThrowIfFailed(hr)
		
		RELPTR(pWarpAdapter)
	}

#if defined(DEBUG) || defined(_DEBUG)
	// DEBUG_CreateInfoQueue();
#endif
}

void D3DRenderer::CreateFenceAndGetDescriptorsSizes()
{
	const HRESULT hr = m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence));
	ThrowIfFailed(hr)

	m_rtvDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_dsvDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_cbvSrvUavDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void D3DRenderer::CheckMSAAQualitySupport()
{
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = m_backBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;

	const HRESULT hr = m_pDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels));
	ThrowIfFailed(hr)

	m_4xMsaaQuality = msQualityLevels.NumQualityLevels;
	m_4xMsaaState = SUCCEEDED(m_pDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels)));
	assert(m_4xMsaaQuality > 0 && "Unexpected MSAA quality level.");
}

void D3DRenderer::CreateSwapChain(const HWND windowHandle)
{
	// Release the previous swapchain we will be recreating.
	if (m_pSwapChain != nullptr) m_pSwapChain->Release();

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = m_bufferWidth;
	sd.BufferDesc.Height = m_bufferHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = m_backBufferFormat;
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
	const HRESULT hr = m_pDxgiFactory->CreateSwapChain(m_pCommandQueue, &sd, &m_pSwapChain);
	ThrowIfFailed(hr)
}

void D3DRenderer::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;

	HRESULT hr = m_pDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_pRtvHeap));
	ThrowIfFailed(hr)

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;

	hr = m_pDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_pDsvHeap));
	ThrowIfFailed(hr)

	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.NumDescriptors = 2000; // NOTE : This number represents the maximum number of textures that can be stored
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;

	hr = m_pDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_pCbvSrvHeap));
	ThrowIfFailed(hr)
}

void D3DRenderer::CreateDepthStencilBuffer()
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
	depthStencilDesc.Format = m_depthStencilFormat;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = m_depthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;

	const CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT); 
	const HRESULT hr = m_pDevice->CreateCommittedResource(&defaultHeapProperties, D3D12_HEAP_FLAG_NONE, &depthStencilDesc, D3D12_RESOURCE_STATE_COMMON, &optClear, IID_PPV_ARGS(&m_pDepthStencilBuffer));
	ThrowIfFailed(hr)
	
	// Create descriptor to mip level 0 of entire resource using the
	// format of the resource.
	m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, DepthStencilView());

	// Apply a resource barrier transition to write a new resource as a depth-stencil buffer.
	// We call this methods before executing the command list to make sure the resource is in the correct state
	const CD3DX12_RESOURCE_BARRIER bCommonToDepthWrite = CD3DX12_RESOURCE_BARRIER::Transition(m_pDepthStencilBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	m_pCommandList->ResourceBarrier(1, &bCommonToDepthWrite);

	EndList();
}

void D3DRenderer::CreateRenderTargetView()
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_pRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		// Get the ith buffer in the swap chain.
		const HRESULT hr = m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pSwapChainBuffer[i]));
		ThrowIfFailed(hr)

		// Create an RTV to it.
		m_pDevice->CreateRenderTargetView(m_pSwapChainBuffer[i], nullptr, rtvHeapHandle);

		// Next entry in heap.
		rtvHeapHandle.Offset(1, m_rtvDescriptorSize);
	}
}
#pragma endregion

#pragma region COMMAND_LIST_PRIVATE
void D3DRenderer::DEBUG_CreateInfoQueue() const
{
	ID3D12InfoQueue* infoQueue = nullptr;
	HRESULT hr = m_pDevice->QueryInterface(IID_PPV_ARGS(&infoQueue));
	ThrowIfFailed(hr)
	hr = infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
	ThrowIfFailed(hr)
	hr = infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
	ThrowIfFailed(hr)
	hr = infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, false);
	ThrowIfFailed(hr)
}

void D3DRenderer::CreateCommandObjects()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	
	HRESULT hr = m_pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pCommandQueue));
	ThrowIfFailed(hr)

	hr = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocator));
	ThrowIfFailed(hr)

	hr = m_pDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_pCommandAllocator, // Associated command allocator
		nullptr, // Initial PipelineStateObject
		IID_PPV_ARGS(&m_pCommandList)
	);
	ThrowIfFailed(hr)

	// Start off in a closed state. This is because the first time we
	// refer to the command list we will Reset it, and it needs to be
	// closed before calling Reset.
	hr = m_pCommandList->Close();
	ThrowIfFailed(hr)
}

void D3DRenderer::FlushCommandQueue()
{
	// Advance the fence value to mark commands up to this fence point.
	m_CurrentFenceValue++;
	
	// Add an instruction to the command queue to set a new fence point. 
	// Because we are on the GPU timeline, the new fence point won't be 
	// set until the GPU finishes processing all the commands prior to this Signal().
	HRESULT hr = m_pCommandQueue->Signal(m_pFence, m_CurrentFenceValue);
	ThrowIfFailed(hr)

	// Wait until the GPU has completed commands up to this fence point.
	if (m_pFence->GetCompletedValue() < m_CurrentFenceValue)  // NOLINT(clang-diagnostic-sign-compare)
	{
		const HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

		// Fire event when GPU hits current fence.  
		hr = m_pFence->SetEventOnCompletion(m_CurrentFenceValue, eventHandle);
		ThrowIfFailed(hr)

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}
#pragma endregion

#pragma region CREATE_INTERNAL_COMPONENTS
void D3DRenderer::CreateResources()
{
	// Create all resources for our engine (shaders and materials)
	Resource::CreateResources(m_pDevice, m_pCbvSrvHeap, m_cbvSrvUavDescriptorSize);

	// Now that all resources are created, we can create the PSO and Root Signature for each shader
	for (const auto& shader : Resource::GetShaders() | std::views::values)
	{
		shader->CreatePsoAndRootSignature(VertexType::VERTEX, m_backBufferFormat, m_depthStencilFormat);
	}
}

void D3DRenderer::GetRenderComponentsRef()
{
	// TODO : CHANGE THIS ASAP
	Coordinator* coordinator = Coordinator::GetInstance();

	m_meshRenderers = coordinator->GetAllComponentsOfType<MeshRenderer>()->GetAllData();
	m_particleRenderers = coordinator->GetAllComponentsOfType<ParticleRenderer>()->GetAllData();
	m_skyRenderers = coordinator->GetAllComponentsOfType<SkyRenderer>()->GetAllData();
	m_uiRenderers = coordinator->GetAllComponentsOfType<UIRenderer>()->GetAllData();
}

void D3DRenderer::CreateFrustum()
{
	BoundingFrustum::CreateFromMatrix(m_frustum, Engine::GetMainCamera()->GetProj());
}
#pragma endregion

#pragma region UPDATE 
int D3DRenderer::UpdateTextureHeap(Texture* tex, int textType)
{
	if (!tex) return -1;

	// Everytime we create a new texture, we need to store it in the m_pCbvSrvHeap.
	// To do so, we create a new descriptor handle and offset it by the number of textures already stored in the heap.
	// The methods returns the index of the texture in the heap, which will be stored in the Texture class.
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(m_pCbvSrvHeap->GetCPUDescriptorHandleForHeapStart());
	hDescriptor.Offset(m_texIndex, m_cbvSrvUavDescriptorSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = static_cast<D3D12_SRV_DIMENSION>(textType);
	srvDesc.Format = tex->Resource->GetDesc().Format;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = tex->Resource->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	m_pDevice->CreateShaderResourceView(tex->Resource, &srvDesc, hDescriptor);

	return m_texIndex++;
}

void D3DRenderer::UpdateRenderedObjects(const float dt, const float totalTime) const
{
	for (UIRenderer* uir : *m_uiRenderers)
	{
		if (!uir) continue;
		uir->Update(dt);
	}

	for (MeshRenderer* mr : *m_meshRenderers)
	{
		if (!mr) continue;
		mr->Update(dt);
	}

	for (ParticleRenderer* pr : *m_particleRenderers)
	{
		if (!pr) continue;
		pr->Update(dt);
	}

	for (SkyRenderer* sr : *m_skyRenderers)
	{
		if (!sr) continue;
		sr->Update(dt);
	}
}

void D3DRenderer::RenderObjects()
{
	// Render the UI first since they are most likely to hide other objects
	for (UIRenderer* uir : *m_uiRenderers)
	{
		if (!uir) continue;
		uir->Render(m_pCommandList);
	}

	// Create the bounding frustum from the camera's projection matrix
	// For more Frustum information, please see the chapter 16.2 and 16.3 of the book
	CreateFrustum();

	const XMMATRIX view = Engine::GetMainCamera()->GetView();
	const XMMATRIX invView = XMMatrixInverse(nullptr, view);

	for (MeshRenderer* mr : *m_meshRenderers)
	{
		if (!mr) continue;

		// Some mesh renderers are not clippable (e.g. spaceship parts)
		if (mr->IsClippable())
		{
			// Create a bounding sphere from the mesh renderer's transform
			// Note that we get the highest scale of the transform to make sure the bounding sphere is big enough
			// This could be improved by using the bounding box instead or multiplying the scale by the mesh radius
			
			BoundingSphere bs;
			bs.Center = mr->transform->GetPosition();
			bs.Radius = mr->transform->GetHighestScale();

			// Create a bounding frustum from the camera's view matrix
			BoundingFrustum viewFrustum;
			m_frustum.Transform(viewFrustum, invView);

			// If the bounding sphere is not in the camera's view, don't render the mesh
			if (viewFrustum.Contains(bs) == DirectX::DISJOINT) continue;
		}

		mr->Render(m_pCommandList);
	}

	// Render the particles after the meshRenderers to avoid depth buffer issues (particles are transparent)
	for (ParticleRenderer* pr : *m_particleRenderers)
	{
		if (!pr) continue;
		pr->Render(m_pCommandList);
	}

	// Render the first registered skybox only
	// We render the skybox at the end as it is the most likely to be hidden by other objects
	if (SkyRenderer* sr = m_skyRenderers->at(0))
	{
		sr->Render(m_pCommandList);
	}

}
#pragma endregion

#pragma region CONSTANTS
D3D12_CPU_DESCRIPTOR_HANDLE D3DRenderer::CurrentBackBufferView() const
{
	// CD3DX12 constructor to offset to the RTV of the current back buffer.
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(),		// handle start
		m_currBackBuffer,										// index to offset
		m_rtvDescriptorSize										// byte size of descriptor
	);
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DRenderer::DepthStencilView() const
{
	return m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();
}
#pragma endregion

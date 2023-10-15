#include "Chokbar.h"
#include "D3DApp.h"
#include <cassert>

using Microsoft::WRL::ComPtr;


D3DApp* D3DApp::m_pApp = nullptr;


D3DApp::D3DApp(HINSTANCE hInstance)
	: Window(L"DX12 App", NULL), m_pInstance(hInstance)
{
	// Only one D3DApp can be constructed.
	assert(m_pApp == nullptr);
	m_pApp = this;
}

D3DApp::~D3DApp()
{
	/*if (md3dDevice != nullptr)
		FlushCommandQueue();
	*/
}

void D3DApp::Initialize()
{
	InitializeWindow();
	InitializeD3D12();
}

void D3DApp::InitializeWindow()
{
	Window::RegisterNewClass();
	Window::Initialize();
}


void D3DApp::InitializeD3D12()
{
#if defined(DEBUG) || defined(_DEBUG)
	EnableDebugLayer();
#endif

	CreateDevice();
	CreateFenceAndDescriptor();
	CheckMSAAQualitySupport();
	CreateSwapChain();
	CreateRtvAndDsvDescriptorHeaps();
	CreateRenderTargetView();
	CreateDepthStencilBuffer();

	m_ScreenViewport.TopLeftX = 0;
	m_ScreenViewport.TopLeftY = 0;
	m_ScreenViewport.Width = static_cast<float>(Size().cx);
	m_ScreenViewport.Height = static_cast<float>(Size().cy);
	m_ScreenViewport.MinDepth = 0.0f;
	m_ScreenViewport.MaxDepth = 1.0f;

	m_ScissorRect = { 0, 0, Size().cx, Size().cy };
}


void D3DApp::EnableDebugLayer()
{
	// Enable the D3D12 debug layer.
	ComPtr<ID3D12Debug> debugController;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
	debugController->EnableDebugLayer();
}

void D3DApp::CreateDevice()
{
	CreateDXGIFactory1(IID_PPV_ARGS(&m_pDxgiFactory));

	HRESULT hardwareResult = D3D12CreateDevice(
		nullptr, // default adapter
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&m_pD3dDevice)
	);

	// Fallback to WARP device.
	if (FAILED(hardwareResult))
	{
		ComPtr<IDXGIAdapter> pWarpAdapter;
		m_pDxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter));
		D3D12CreateDevice(
			pWarpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&m_pD3dDevice)
		);
	}
}

void D3DApp::CreateFenceAndDescriptor()
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
	assert(m_4xMsaaQuality > 0 && "Unexpected MSAA quality level.");
}

void D3DApp::CreateCommandObjects()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	m_pD3dDevice->CreateCommandQueue(
		&queueDesc, IID_PPV_ARGS(&m_pCommandQueue)
	);

	m_pD3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(m_pCommandAllocator.GetAddressOf())
	);

	m_pD3dDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_pCommandAllocator.Get(), // Associated command allocator
		nullptr, // Initial PipelineStateObject
		IID_PPV_ARGS(m_pCommandList.GetAddressOf())
	);

	// Start off in a closed state. This is because the first time we
	// refer to the command list we will Reset it, and it needs to be
	// closed before calling Reset.
	m_pCommandList->Close();
}

void D3DApp::CreateSwapChain()
{
	// Release the previous swapchain we will be recreating.
	m_pSwapChain.Reset();

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = Size().cx;
	sd.BufferDesc.Height = Size().cy;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = m_BackBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
	sd.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT;
	sd.OutputWindow = Handle();
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
	m_pDxgiFactory->CreateSwapChain(
		m_pCommandQueue.Get(),
		&sd,
		m_pSwapChain.GetAddressOf()
	);
}

void D3DApp::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;

	m_pD3dDevice->CreateDescriptorHeap(
		&rtvHeapDesc, IID_PPV_ARGS(m_pRtvHeap.GetAddressOf())
	);


	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;

	m_pD3dDevice->CreateDescriptorHeap(
		&dsvHeapDesc, IID_PPV_ARGS(m_pDsvHeap.GetAddressOf())
	);
}

void D3DApp::CreateRenderTargetView()
{
	// Release the previous resources we will be recreating.
	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
		m_pSwapChainBuffer[i].Reset();
	m_pDepthStencilBuffer.Reset();

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_pRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		// Get the ith buffer in the swap chain.
		m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pSwapChainBuffer[i]));

		// Create an RTV to it.
		m_pD3dDevice->CreateRenderTargetView(m_pSwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);

		// Next entry in heap.
		rtvHeapHandle.Offset(1, m_RtvDescriptorSize);
	}
}

void D3DApp::CreateDepthStencilBuffer()
{
	// Create the depth/stencil buffer and view.
	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = Size().cx;
	depthStencilDesc.Height = Size().cy;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = m_DepthStencilFormat;
	depthStencilDesc.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
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
		IID_PPV_ARGS(m_pDepthStencilBuffer.GetAddressOf())
	);

	// Create descriptor to mip level 0 of entire resource using the
	// format of the resource.
	m_pD3dDevice->CreateDepthStencilView(
		m_pDepthStencilBuffer.Get(),
		nullptr,
		DepthStencilView());

	// Transition the resource from its initial state to be used as a depth buffer.
	m_pCommandList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			m_pDepthStencilBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_DEPTH_WRITE
		)
	);
}


D3D12_CPU_DESCRIPTOR_HANDLE D3DApp::CurrentBackBufferView() const
{
	// CD3DX12 constructor to offset to the RTV of the current back buffer.
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(),		// handle start
		mCurrBackBuffer,										// index to offset
		m_RtvDescriptorSize										// byte size of descriptor
	); 
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DApp::DepthStencilView() const
{
	return m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();
}
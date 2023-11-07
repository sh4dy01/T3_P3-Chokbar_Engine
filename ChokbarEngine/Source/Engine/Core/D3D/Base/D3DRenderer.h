#pragma once

#include "D3DUtils.h"

#pragma region GLOBAL VARIABLES

static const int SWAP_CHAIN_BUFFER_COUNT = 2;

#pragma endregion

class MeshRenderer;
class ParticleRenderer;
class Texture;

class D3DRenderer
{
public:
	D3DRenderer();
	~D3DRenderer();

public:
	static D3DRenderer* GetInstance();

	void InitializeD3D12(Win32::Window* window);
	void OnResize(int, int);
	void Update(const float dt, const float totalTime);
	void Render();

	ID3D12Device* GetDevice() const { return m_pD3dDevice; }
	ID3D12GraphicsCommandList* GetCommandList() const { return m_pCommandList; }
	ID3D12DescriptorHeap* GetCbvHeap() const { return m_pCbvHeap; }
	void BeginList();
	void EndList();

	int UpdateTextureHeap(Texture* tex, int textType);


	int m_bufferWidth;
	int m_bufferHeight;

private:
	void EnableDebugLayer();

	void CreateDevice();
	void CheckMSAAQualitySupport();

	void CreateFenceAndGetDescriptorsSizes();

	void CreateCommandObjects();
	void FlushCommandQueue();

	void CreateSwapChain(HWND windowHandle);

	void CreateRtvAndDsvDescriptorHeaps();
	void CreateRenderTargetView();
	void CreateDepthStencilBuffer();

	void CreateResources();

	void GetRenderComponentsRef();
	void RenderObjects();
	void UpdateRenderedObjects(const float dt, const float totalTime);

	void CreateFustrum();

	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

	/* Creates an ID3D12InfoQueue to catch any error within the Command Queue.
	Any error will break the code */
	void DEBUG_CreateInfoQueue();

private:
	static D3DRenderer* m_pApp;

	HINSTANCE m_pInstance;

	ID3D12Debug* m_pDebugController;

	bool m_4xMsaaState;	  // 4X MSAA (4.1.8) enabled. Default is false.
	UINT m_4xMsaaQuality; // quality level of 4X MSAA


	/* D3D12 Factory : Used to create the swap chain */
	IDXGIFactory4* m_pDxgiFactory;

	/* D3D12 Device : Represents a GPU device */
	ID3D12Device* m_pD3dDevice;
	D3D_DRIVER_TYPE m_D3dDriverType;
	/* D3D12 Fence : Used to synchronize the CPU and GPU
	We use the fence to wait for the CPU or GPU to finish their work.
	The other one will be put on hold */
	ID3D12Fence* m_pFence;
	int m_CurrentFenceValue;

	/* D3D12 CommandQueue : Represents the actual command queue of the GPU */
	ID3D12CommandQueue* m_pCommandQueue;
	/* D3D12 CommandAllocator : Represents a chunk of memory on the GPU that stores commands */
	ID3D12CommandAllocator* m_pCommandAllocator;
	/* D3D12 GraphicsCommandList : Represents a list of commands that will be sent to the GPU command queue */
	ID3D12GraphicsCommandList* m_pCommandList;
	/* D3D12 RenderTargetView DescriptorHeap :  */
	ID3D12DescriptorHeap* m_pRtvHeap;
	/* D3D12 DepthStencilView DescriptorHeap :  */
	ID3D12DescriptorHeap* m_pDsvHeap;
	/* D3D12 ConstantBufferView DescriptorHeap :  */
	ID3D12DescriptorHeap* m_pCbvHeap;

	/* Speaks for itself I guess */
	UINT m_RtvDescriptorSize;
	UINT m_DsvDescriptorSize;
	UINT m_CbvSrvUavDescriptorSize;

	/* D3D12 SwapChain : Used to swap the back buffer */
	IDXGISwapChain* m_pSwapChain;
	/* Buffer used by the swap chain (contains our two buffers that will serve for the Present() method) */
	ID3D12Resource* m_pSwapChainBuffer[SWAP_CHAIN_BUFFER_COUNT];
	/* Index of the current back buffer */
	int m_currBackBuffer;
	DXGI_FORMAT m_BackBufferFormat;

	/* The depth stencil buffer is used to render perspective given the object position
	This results with objects appearing to be behind each others */
	ID3D12Resource* m_pDepthStencilBuffer;
	DXGI_FORMAT m_DepthStencilFormat;

	std::array<MeshRenderer*, MAX_ENTITIES>* m_meshRenderers;
	std::array<ParticleRenderer*, MAX_ENTITIES>* m_particleRenderers;
	std::array<SkyRenderer*, MAX_ENTITIES>* m_skyRenderers;

	UINT m_texIndex;

	DirectX::BoundingFrustum m_Frustum;
};

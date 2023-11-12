#pragma once

#pragma region GLOBAL VARIABLES

static constexpr int SWAP_CHAIN_BUFFER_COUNT = 2;

#pragma endregion

class MeshRenderer;
class ParticleRenderer;
class Texture;

/*
Base render class for the D3D12 API.
It contains all the necessary components and methods to use the D3D12 API.
This class is a singleton, you can get any DX12 component from it.
*/
class D3DRenderer
{
public:
	D3DRenderer();
	~D3DRenderer();

public:
	static D3DRenderer* GetInstance();

	void InitializeD3D12(const Win32::Window* window);
	void OnResize(int, int);
	void Update(const float dt, const float totalTime) const;
	void Render();

	ID3D12Device* GetDevice() const { return m_pDevice; }
	ID3D12GraphicsCommandList* GetCommandList() const { return m_pCommandList; }
	ID3D12DescriptorHeap* GetCbvHeap() const { return m_pCbvSrvHeap; }
	void BeginList() const;
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
	void UpdateRenderedObjects(const float dt, const float totalTime) const;

	void CreateFrustum();

	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

	/* Creates an ID3D12InfoQueue to catch any error within the Command Queue.
	Any error will break the code */
	void DEBUG_CreateInfoQueue() const;

private:
	static D3DRenderer* m_pApp;

	HINSTANCE m_pInstance;

	ID3D12Debug* m_pDebugController;

	bool m_4xMsaaState;	  // 4X MSAA (4.1.8) enabled. Default is false.
	UINT m_4xMsaaQuality; // quality level of 4X MSAA


	/* D3D12 Factory : Used to create the swap chain */
	IDXGIFactory4* m_pDxgiFactory;

	/* D3D12 Device : Represents a GPU device */
	ID3D12Device* m_pDevice;
	D3D_DRIVER_TYPE m_driveType;
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
	/*
	D3D12 ConstantBufferView DescriptorHeap : The cbv heap is used to store every resource needed by the shaders. We store the constant buffers, the textures or samplers
	The DX12 approach is to store everything in a single heap, but we can prevent to store constant buffers and samplers. This methods will be further explained in the ShaderBase class
	*/
	ID3D12DescriptorHeap* m_pCbvSrvHeap;

	/* Speaks for itself I guess */
	UINT m_rtvDescriptorSize;
	UINT m_dsvDescriptorSize;
	UINT m_cbvSrvUavDescriptorSize;

	/* D3D12 SwapChain : Used to swap the back buffer */
	IDXGISwapChain* m_pSwapChain;
	/* Buffer used by the swap chain (contains our two buffers that will serve for the Present() method) */
	ID3D12Resource* m_pSwapChainBuffer[SWAP_CHAIN_BUFFER_COUNT];
	/* Index of the current back buffer */
	int m_currBackBuffer;
	DXGI_FORMAT m_backBufferFormat;

	/* The depth stencil buffer is used to render perspective given the object position
	This results with objects appearing to be behind each others */
	ID3D12Resource* m_pDepthStencilBuffer;
	DXGI_FORMAT m_depthStencilFormat;

	/*
	These arrays store renderers of different types. Each renderer is associated with an entity.
	Some renderers like MeshRenderers needs to be clipped by the frustum, so we differentiate them from the others
	TODO : Implemented frustum clipping for ParticleRenderers
	*/
	std::array<MeshRenderer*, MAX_ENTITIES>* m_meshRenderers;
	std::array<ParticleRenderer*, MAX_ENTITIES>* m_particleRenderers;
	std::array<SkyRenderer*, MAX_ENTITIES>* m_skyRenderers;
	std::array<UIRenderer*, MAX_ENTITIES>* m_uiRenderers;

	/*
	Keeps track of how many textures were created in the CBV/SRV/UAV heap.
	TODO : Move this the Resource class
	*/
	int m_texIndex;

	/* The frustum is used to clip objects that are not in the camera's view */
	DirectX::BoundingFrustum m_frustum;
};

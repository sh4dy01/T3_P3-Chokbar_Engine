#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <Core/d3dx12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#include "UploadBuffer.h"
#include "MeshGeometry.h"

#include <vector>
#include <unordered_map>

#pragma region GLOBAL VARIABLES

static const int SWAP_CHAIN_BUFFER_COUNT = 2;

/* ------------------------------------------------------------------------- */
/* HELPER STRUCTS                                                            */
/* ------------------------------------------------------------------------- */
#pragma region Helper Structs
struct CHOKBAR_API Vertex
{
	XMFLOAT3 Pos;
	UINT32 Color;
};

#pragma endregion

class D3DApp
{
public:
	D3DApp();
	~D3DApp();

public:
	static D3DApp *GetInstance();

	void InitializeD3D12(Win32::Window *window);
	void OnResize(int, int);
	void Update(const float dt, const float totalTime);
	void Render();

private:
	void EnableDebugLayer();

	void CreateDevice();
	void CheckMSAAQualitySupport();

	void CreateFenceAndGetDescriptorsSizes();

	void CreateCommandObjects();
	void FlushCommandQueue();

	void CreateSwapChain(HWND windowHandle);

	void RegisterInitCommands_In_CommandList();
	void CreateRtvAndDsvDescriptorHeaps();
	void CreateRenderTargetView();
	void CreateDepthStencilBuffer();

	void CreateGeometry();
	void CreateConstantBuffers();
	void UpdateObjectCB(const float dt, const float totalTime);
	void UpdateMainPassCB(const float dt, const float totalTime);

	void CreateObject();
	void CreateRenderItems();
	void DrawRenderItems(ID3D12GraphicsCommandList *cmdList, const std::vector<RenderItem *> &renderItems);

	void CreateRootSignature();
	void CreatePipelineStateObject();

	/* /!\ Be careful, this method uses the commandList component.
	Therefore, it must be called within a command list Reset() and ExecuteCommandList() scope */
	ID3D12Resource *CreateDefaultBuffer(const void *initData, UINT64 byteSize, ID3D12Resource *uploadBuffer);
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;
	ID3DBlob *CompileShader(const std::wstring &filename, const D3D_SHADER_MACRO *defines, const std::string &entrypoint, const std::string &target);

	/* Creates an ID3D12InfoQueue to catch any error within the Command Queue.
	Any error will break the code */
	void DEBUG_CreateInfoQueue();

private:
	static D3DApp *m_pApp;

	HINSTANCE m_pInstance;

	ID3D12Debug *m_pDebugController;

	bool m_4xMsaaState;	  // 4X MSAA (4.1.8) enabled. Default is false.
	UINT m_4xMsaaQuality; // quality level of 4X MSAA

	int m_bufferWidth;
	int m_bufferHeight;

	/* D3D12 Factory : Used to create the swap chain */
	IDXGIFactory4 *m_pDxgiFactory;

	/* D3D12 Device : Represents a GPU device */
	ID3D12Device *m_pD3dDevice;
	D3D_DRIVER_TYPE m_D3dDriverType;
	/* D3D12 Fence : Used to synchronize the CPU and GPU
	We use the fence to wait for the CPU or GPU to finish their work.
	The other one will be put on hold */
	ID3D12Fence *m_pFence;
	int m_CurrentFenceValue;

	/* D3D12 CommandQueue : Represents the actual command queue of the GPU */
	ID3D12CommandQueue *m_pCommandQueue;
	/* D3D12 CommandAllocator : Represents a chunk of memory on the GPU that stores commands */
	ID3D12CommandAllocator *m_pCommandAllocator;
	/* D3D12 GraphicsCommandList : Represents a list of commands that will be sent to the GPU command queue */
	ID3D12GraphicsCommandList *m_pCommandList;
	/* D3D12 RenderTargetView DescriptorHeap :  */
	ID3D12DescriptorHeap *m_pRtvHeap;
	/* D3D12 DepthStencilView DescriptorHeap :  */
	ID3D12DescriptorHeap *m_pDsvHeap;
	/* D3D12 ConstantBufferView DescriptorHeap :  */
	ID3D12DescriptorHeap *m_pCbvHeap;

	/* Speaks for itself I guess */
	UINT m_RtvDescriptorSize;
	UINT m_DsvDescriptorSize;
	UINT m_CbvSrvUavDescriptorSize;

	/* This struct helps the GPU identifying how our Vertex class is composed */
	D3D12_INPUT_ELEMENT_DESC m_inputLayout[2];

	/* D3D12 SwapChain : Used to swap the back buffer */
	IDXGISwapChain *m_pSwapChain;
	/* Buffer used by the swap chain (contains our two buffers that will serve for the Present() method) */
	ID3D12Resource *m_pSwapChainBuffer[SWAP_CHAIN_BUFFER_COUNT];
	/* Index of the current back buffer */
	int m_currBackBuffer;
	DXGI_FORMAT m_BackBufferFormat;

	/* The depth stencil buffer is used to render perspective given the object position
	This results with objects appearing to be behind each others */
	ID3D12Resource *m_pDepthStencilBuffer;
	DXGI_FORMAT m_DepthStencilFormat;

	/* All our items to be rendered in our world. They are not sorted by shader.*/
	std::vector<RenderItem> m_AllRenderItems{};
	/* All opaque item in our world. They might not use the same shader as well.
	RenderItem* points to a render item in m_AllRenderItems
	/!\ Do not create an object directly into this list, you need to create it in m_AllItem and add the reference in this list */
	std::vector<RenderItem *> m_OpaqueRenderItems{};
	/* All transparent item in our world. They might not use the same shader as well.
	RenderItem* points to a render item in m_AllRenderItems
	/!\ Do not create an object directly into this list, you need to create it in m_AllItem and add the reference in this list */
	std::vector<RenderItem *> m_TransparentRenderItems{};

	/* Reference to our pyramid geometry. It is instanciated once and can be used by any RenderItem */
	MeshGeometry *m_pyramidGeometry;

	const int m_ObjectCount = 2;

	/* Upload buffers are used to give the GPU information at runtime with the CPU.
	Those buffers uses the GPU Upload Heap that allows the CPU to upload data to the GPU at runtime */

	/* The Main Object Constant Buffer stocks every constant buffer. Each constant buffer is associated to an unique RenderItem
	To find the associated RenderItem, you can use the index of the used object constant buffer
	NOTE : The object constant buffer is associated to the b0 cbuffer in the shader (only true in our project) */
	std::vector<UploadBuffer<ObjectConstants> *> m_mainObjectCB;
	/* The Main Pass Constant Buffer stores every information the shader might need about our camera
	NOTE : The main pass constant buffer is associated to the b1 cbuffer in the shader (only true in our project) */
	UploadBuffer<PassConstants> *m_mainPassCB;

	/* Compile code of the vertex shader */
	ID3DBlob *m_vsByteCode;
	/* Compile code of the pixel shader */
	ID3DBlob *m_psByteCode;
	/* D3D12RootSignature : Defines where the resources bound to the rendering pipeline can be found by the shader
	We use a root signature to define the resources that are going to be used by the shaders
	Therefore, the root signature will be created with an array of RootParameter that express where the exprected resource by the shader is located */
	ID3D12RootSignature *m_rootSignature;

	/* First implementation of a Camera object */
	Camera m_camera{};

	/* D3D12 PipelineStateObject : (PSO : Pipeline State Object) Represents the state of the pipeline
	We use a PSO to define the state of the pipeline. This includes the shaders, the input layout, the render targets, the depth stencil buffer, etc...
	For each shader, we need to create another PSO, this sytem will be implemented later on */
	enum PSO_TYPE
	{
		PSO_OPAQUE,
		PSO_TRANSPARENT,
		PSO_COUNT
	};
	std::unordered_map<PSO_TYPE, ID3D12PipelineState *> m_PSOs;
	bool m_isWireframe;
};

#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "Engine/GameTimer.h"
#include "Engine/Simulation.h"

#include "UploadBuffer.h"
#include <iostream>
#include <vector>

class MeshGeometry;
class FrameResource;


class CHOKBAR_API D3DApp : public Chokbar::Simulation {

public:

	D3DApp();
	~D3DApp();


public:

	static D3DApp* GetInstance() { return m_pApp; }

	void Initialize() override;

	void Run() override;

	void OnResize();

protected:
	
	void Update(const float dt) override;
	void Render() override;

private:

	void InitializeD3D12();
	void InitializeWindow();

	void EnableDebugLayer();
	
	void CreateDevice();
	void CheckMSAAQualitySupport();
	
	void CreateFenceAndGetDescriptorsSizes();

	void CreateCommandObjects();
	void CreateFrameResources();
	void FlushCommandQueue();

	void CreateSwapChain();

	void RegisterInitCommands_In_CommandList();
	void CreateRtvAndDsvDescriptorHeaps();
	void CreateRenderTargetView();
	void CreateDepthStencilBuffer();
	void CreateVertexAndIndexBuffers();
	void CreateConstantBuffers();
	void UpdateObjectCB(const float dt);
	void UpdateMainPassCB(const float dt, const float totalTime);
	void CreateRootSignature();
	void CreatePipelineStateObject();

	void CalculateFrameStats();

	ID3D12Resource* CreateDefaultBuffer(const void* initData, UINT64 byteSize, ID3D12Resource* uploadBuffer);
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;
	ID3DBlob* CompileShader(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target);

	/* Creates an ID3D12InfoQueue to catch any error within the Command Queue.
	Any error will break the code */
	void DEBUG_CreateInfoQueue();
private:

	static D3DApp *m_pApp;

	// TODO : Move this instance out of this class
	// Move this to the Simulation class
	GameTimer m_GameTimer;

	HINSTANCE m_pInstance;

	ID3D12Debug* m_pDebugController;

	bool		m_4xMsaaState;		// 4X MSAA (4.1.8) enabled. Default is false.
	UINT		m_4xMsaaQuality;	// quality level of 4X MSAA

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
	std::vector<FrameResource*> m_pFrameResources;
	FrameResource* m_pCurrentFrameResource;
	int m_CurrentFrameResourceIndex;
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
	
	/* This struct helps the GPU identifying how our Vertex class is composed */
	D3D12_INPUT_ELEMENT_DESC m_inputLayout[2];

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
	
	std::vector<std::unique_ptr<RenderItem>> m_AllRenderItems;
	std::vector<RenderItem*> m_OpaqueRenderItems;
	std::vector<RenderItem*> m_TransparentRenderItems;
	
	std::unique_ptr<MeshGeometry> m_geometry;

	/* Upload buffer used to give the GPU information at runtime with the CPU.
	This buffer uses the GPU Upload Heap that allows the CPu to upload data to the GPU at runtime */
	std::unique_ptr<UploadBuffer<ObjectConstants>> m_mainObjectCB;
	PassConstants m_mainPassCB;
	/* Compile code of the vertex shader */
	ID3DBlob* m_vsByteCode;
	/* Compile code of the pixel shader */
	ID3DBlob* m_psByteCode;
	/* D3D12RootSignature : Defines where the resources bound to the rendering pipeline can be found by the shader
	We use a root signature to define the resources that are going to be used by the shaders
	Therefore, the root signature will be created with an array of RootParameter that express where the exprected resource by the shader is located */
	ID3D12RootSignature* m_rootSignature;

	XMFLOAT3 m_eyePosition;
	XMFLOAT4X4 m_view, m_proj;
	float m_zRotation;

	/* D3D12 PipelineStateObject : (PSO : Pipeline State Object) Represents the state of the pipeline
	We use a PSO to define the state of the pipeline. This includes the shaders, the input layout, the render targets, the depth stencil buffer, etc... */
	std::unordered_map<std::string, ID3D12PipelineState*> m_PSOs;
	bool m_isWireframe;
};
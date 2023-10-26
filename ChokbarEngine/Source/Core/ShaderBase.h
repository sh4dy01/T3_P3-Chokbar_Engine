#pragma once

#include "UploadBuffer.h"
#include "MeshGeometry.h"

struct Texture;

enum VertexType { POS, POS_COLOR, POS_TEX, POS_NORM_TEX, POS_NORM_TEX_TAN };
enum ShaderType { BASE, SIMPLE, TEXTURE };

struct ShaderDrawArguments
{
	ShaderDrawArguments(ID3D12GraphicsCommandList* cmdList, UINT renderItemCBIndex, MeshGeometry* renderItemGeometry) : CmdList(cmdList), RenderItemCBIndex(renderItemCBIndex), RenderItemGeometry(renderItemGeometry) { }
	ID3D12GraphicsCommandList* CmdList;
	UINT RenderItemCBIndex;
	MeshGeometry* RenderItemGeometry;
};

class ShaderBase
{
public:
	ShaderBase(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvDescriptorSize, std::wstring& filepath);
	virtual ~ShaderBase();

	const ShaderType Type;

protected:

	struct ObjConstants
	{
		DirectX::XMFLOAT4X4 World = Identity4x4();
	};

	struct PassConstants
	{
		DirectX::XMFLOAT4X4 View = Identity4x4();
		// DirectX::XMFLOAT4X4 InvView = Identity4x4();
		DirectX::XMFLOAT4X4 Proj = Identity4x4();
		// DirectX::XMFLOAT4X4 InvProj = Identity4x4();
		DirectX::XMFLOAT4X4 ViewProj = Identity4x4();
		// DirectX::XMFLOAT4X4 InvViewProj = Identity4x4();

		DirectX::XMFLOAT3 EyePosW = { 0.0f, 0.0f, 0.0f };
		// float cbPerObjectPad1 = 0.0f;
		// DirectX::XMFLOAT2 RenderTargetSize = { 0.0f, 0.0f };
		// DirectX::XMFLOAT2 InvRenderTargetSize = { 0.0f, 0.0f };
		// float NearZ = 0.0f;
		// float FarZ = 0.0f;
		float TotalTime = 0.0f;
		float DeltaTime = 0.0f;
	};

protected:
	std::wstring& m_filepath;

	/* Upload buffers are used to give the GPU information at runtime with the CPU.
	Those buffers uses the GPU Upload Heap that allows the CPU to upload data to the GPU at runtime.

	The Main Object Constant Buffer stocks every constant buffer. Each constant buffer is associated to an unique RenderItem
	To find the associated RenderItem, you can use the index of the used object constant buffer
	NOTE : The object constant buffer is associated to the b0 cbuffer in the shader (only true in our project) */
	std::vector<UploadBuffer<ObjConstants>*> m_objectCBs;
	/* The Main Pass Constant Buffer stores every information the shader might need about our camera
	NOTE : The main pass constant buffer is associated to the b1 cbuffer in the shader (only true in our project) */
	UploadBuffer<PassConstants>* m_passCB;
	/* This struct helps the GPU identifying how our Vertex class is composed */
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_inputLayout;

	/* D3D12RootSignature : Defines where the resources bound to the rendering pipeline can be found by the shader
	We use a root signature to define the resources that are going to be used by the shaders
	Therefore, the root signature will be created with an array of RootParameter that express where the exprected resource by the shader is located */
	ID3D12RootSignature* m_rootSignature;

	ID3D10Blob* m_vsByteCode;
	ID3D10Blob* m_psByteCode;

	/* D3D12 PipelineStateObject : (PSO : Pipeline State Object) Represents the state of the pipeline
	We use a PSO to define the state of the pipeline. This includes the shaders, the input layout, the render targets, the depth stencil buffer, etc...
	For each shader, we need to create another PSO, this sytem will be implemented later on */
	ID3D12PipelineState* m_pipelineState;

	ID3D12Device* m_generalDevice;
	ID3D12DescriptorHeap* m_generalCBVHeap;
	UINT m_cbvDescriptorSize;

	Camera* m_generalCamera;

public:
	virtual void Init() = 0;
	virtual void CreatePsoAndRootSignature(VertexType vertexType, DXGI_FORMAT& rtvFormat, DXGI_FORMAT& dsvFormat) = 0;

	virtual void BeginDraw(ID3D12GraphicsCommandList* cmdList) = 0;
	virtual void Draw(ShaderDrawArguments& args, Texture* text = nullptr) = 0;
	virtual void EndDraw(ID3D12GraphicsCommandList* cmdList) = 0;

	UINT GetLastIndex() { return (UINT)m_objectCBs.size(); }
	ShaderBase* Bind();

	virtual void AddObjectCB();
	virtual void UpdateObjectCB(DirectX::XMMATRIX& itemWorldMatrix, UINT cbIndex);

	void CreatePassCB();
	void UpdatePassCB(const float dt, const float totalTime);

	void CompileShader(const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target, ID3DBlob** uploader);

protected:
	void SetInputLayout(VertexType vertexType);
};

class ShaderSimple : public ShaderBase
{
public:
	ShaderSimple(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvDescriptorSize, std::wstring& filepath);
	~ShaderSimple();

	void Init() override;
	void CreatePsoAndRootSignature(VertexType vertexType, DXGI_FORMAT& rtvFormat, DXGI_FORMAT& dsvFormat) override;

	void BeginDraw(ID3D12GraphicsCommandList* cmdList) override;
	void Draw(ShaderDrawArguments& args, Texture* text = nullptr) override;
	void EndDraw(ID3D12GraphicsCommandList* cmdList) override;
};

class ShaderTexture : public ShaderBase
{
public:
	ShaderTexture(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvDescriptorSize, std::wstring& filepath);
	~ShaderTexture();

	void Init() override;
	void CreatePsoAndRootSignature(VertexType vertexType, DXGI_FORMAT& rtvFormat, DXGI_FORMAT& dsvFormat) override;

	void BeginDraw(ID3D12GraphicsCommandList* cmdList) override;
	void Draw(ShaderDrawArguments& args, Texture* text = nullptr) override;
	void EndDraw(ID3D12GraphicsCommandList* cmdList) override;

public:
	void BindTexture(Texture* tex) { m_texture = tex; }

private:
	Texture* m_texture;
};
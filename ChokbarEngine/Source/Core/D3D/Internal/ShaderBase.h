#pragma once
#include <stack>

#include "UploadBuffer.h"
#include "Core/D3D/Internal/D3DMesh.h"

struct Texture;

enum VertexType { VERTEX, PARTICLE };

struct ShaderDrawArguments
{
	ShaderDrawArguments() : ItemCBIndex(-1), IndexCount(0), StartIndexLocation(0), BaseVertexLocation(0)
	{
		CmdList = nullptr;
		ItemGeometry = nullptr;
	}

	ID3D12GraphicsCommandList* CmdList;

	UINT ItemCBIndex;
	D3DMesh* ItemGeometry;
	UINT IndexCount;
	UINT StartIndexLocation;
	UINT BaseVertexLocation;

	UINT TextSrvIndex;
};

class ShaderBase
{
public:
	ShaderBase(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvDescriptorSize, std::wstring& filepath);
	virtual ~ShaderBase();

protected:
	struct ObjConstants
	{
		DirectX::XMFLOAT4X4 World;
	};

	struct PassConstants
	{
		DirectX::XMFLOAT4X4 View;
		DirectX::XMFLOAT4X4 Proj;
		DirectX::XMFLOAT4X4 ViewProj;

		DirectX::XMFLOAT3 EyePosW = { 0.0f, 0.0f, 0.0f };
		float TotalTime = 0.0f;
		float DeltaTime = 0.0f;
	};

	struct MaterialConstants
	{
		DirectX::XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
		float Roughness = .25f;

		// Used in texture mapping.
		DirectX::XMFLOAT4X4 MatTransform = Identity4x4();
	};

protected:
	std::wstring& m_filepath;

	/* Upload buffers are used to give the GPU information at runtime with the CPU.
	Those buffers uses the GPU Upload Heap that allows the CPU to upload data to the GPU at runtime.

	m_objectCBs stores every constant buffer. Each constant buffer is associated to an unique MeshRenderer
	NOTE : The object constant buffer is associated to the b0 cbuffer in the shader (only true in our project) */
	std::vector<UploadBuffer<ObjConstants>*> m_objectCBs;
	std::stack<UINT> m_freeIndices;
	/* m_passCB stores every information the shader might need about our camera
	NOTE : The main pass constant buffer is associated to the b1 cbuffer in the shader (only true in our project) */
	UploadBuffer<PassConstants>* m_passCB;
	/* This struct helps the GPU identifying how our Vertex class is composed
	This information will be used by the shader as the VertexIn struct*/
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
	UINT m_cbvDescriptorSize;

	CameraComponent* m_MainCamera;

public:
	virtual void Init() = 0;
	virtual void CreatePsoAndRootSignature(VertexType vertexType, DXGI_FORMAT& rtvFormat, DXGI_FORMAT& dsvFormat) = 0;

	virtual void BeginDraw(ID3D12GraphicsCommandList* cmdList) = 0;
	virtual void Draw(ShaderDrawArguments& args) = 0;
	virtual void EndDraw(ID3D12GraphicsCommandList* cmdList) = 0;

	UINT GetCreatedIndex() { return (UINT)m_objectCBs.size() - 1; }
	UINT GetLastIndex() { return (UINT)m_objectCBs.size(); }

	void UnBind(UINT index);
	ShaderBase* Bind();

	virtual void AddObjectCB();
	virtual void UpdateObjectCB(DirectX::XMFLOAT4X4* itemWorldMatrix, UINT cbIndex);

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
	void Draw(ShaderDrawArguments& args) override;
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
	void Draw(ShaderDrawArguments& args) override;
	void EndDraw(ID3D12GraphicsCommandList* cmdList) override;

private:
	/* Samplers defines how we want to parse our texture in our shader.
	Creating multiple Samplers allows us to have access to different parse mode in our shader later on. This is useful on big engines.
	Note that will certainly do not need that much samplers, but it was juste to learn how to use them. */
	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();
};

class ShaderParticle : public ShaderBase
{
public:
	struct InstanceData
	{
		DirectX::XMFLOAT4X4 World = Identity4x4();
		float Age = 0.0f;

		DirectX::XMFLOAT3 Velocity = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 Rotation = { 0.0f, 0.0f, 0.0f };
		float LifeTime = 0.0f;
	};

public:
	ShaderParticle(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvDescriptorSize, std::wstring& filepath);
	~ShaderParticle();

	void Init() override;
	void CreatePsoAndRootSignature(VertexType vertexType, DXGI_FORMAT& rtvFormat, DXGI_FORMAT& dsvFormat) override;

	void BeginDraw(ID3D12GraphicsCommandList* cmdList) override;
	void Draw(ShaderDrawArguments& args) override;
	void EndDraw(ID3D12GraphicsCommandList* cmdList) override;

private:
};
#pragma once
#include <stack>

#include "UploadBuffer.h"

struct Texture;
class ParticleRenderer;
class MeshRenderer;
struct InstanceData;

enum VertexType { VERTEX };

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
		DirectX::XMFLOAT4X4 View = Identity4x4();
		DirectX::XMFLOAT4X4 Proj = Identity4x4();
		DirectX::XMFLOAT4X4 ViewProj = Identity4x4();

		DirectX::XMFLOAT4 LightColor = { 1.0f, 0.0f, 1.0f, 1.0f };

		DirectX::XMFLOAT3 EyePosW = { 0.0f, 0.0f, 0.0f };
		float TotalTime = 0.0f;

		DirectX::XMFLOAT3 LightDirection = { -1.0f, -1.0f, 0.0f };
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

public:
	virtual void Init() = 0;
	virtual void CreatePsoAndRootSignature(VertexType vertexType, DXGI_FORMAT& rtvFormat, DXGI_FORMAT& dsvFormat) = 0;

	virtual void BeginDraw(ID3D12GraphicsCommandList* cmdList) = 0;
	virtual void Draw(ID3D12GraphicsCommandList* cmdList, IRenderer* drawnMeshR) = 0;
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

	/* Samplers defines how we want to parse our texture in our shader.
	Creating multiple Samplers allows us to have access to different parse mode in our shader later on. This is useful on big engines.
	Note that will certainly do not need that much samplers, but it was juste to learn how to use them. */
	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 2> GetStaticSamplers();
};

class ShaderSimple : public ShaderBase
{
public:
	ShaderSimple(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvDescriptorSize, std::wstring& filepath);
	~ShaderSimple();

	void Init() override;
	void CreatePsoAndRootSignature(VertexType vertexType, DXGI_FORMAT& rtvFormat, DXGI_FORMAT& dsvFormat) override;

	void BeginDraw(ID3D12GraphicsCommandList* cmdList) override;
	void Draw(ID3D12GraphicsCommandList* cmdList, IRenderer* drawnMeshR) override;
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
	void Draw(ID3D12GraphicsCommandList* cmdList, IRenderer* drawnMeshR) override;
	void EndDraw(ID3D12GraphicsCommandList* cmdList) override;
};

class ShaderParticle : public ShaderBase
{
public:
	ShaderParticle(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvDescriptorSize, std::wstring& filepath);
	~ShaderParticle();

	void Init() override;
	void CreatePsoAndRootSignature(VertexType vertexType, DXGI_FORMAT& rtvFormat, DXGI_FORMAT& dsvFormat) override;

	void BeginDraw(ID3D12GraphicsCommandList* cmdList) override;
	void Draw(ID3D12GraphicsCommandList* cmdList, IRenderer* drawnMeshR) override;
	void EndDraw(ID3D12GraphicsCommandList* cmdList) override;

	void UpdateParticleInstanceDataBuffer(int startIndex, const void* data);
private:
	void DrawAsParticle(ID3D12GraphicsCommandList* cmdList, ParticleRenderer* drawnMeshR);

	UploadBuffer<InstanceData>* m_particleInstanceDataBuffer;
};

class ShaderSkybox : public ShaderTexture
{
public:
	ShaderSkybox(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvDescriptorSize, std::wstring& filepath);
	~ShaderSkybox();

	void CreatePsoAndRootSignature(VertexType vertexType, DXGI_FORMAT& rtvFormat, DXGI_FORMAT& dsvFormat) override;
};

class ShaderTextureTransparent : public ShaderBase
{
public:
	struct OffSetConstants
	{
		DirectX::XMFLOAT4X4 World;
		float UVOffsetY = 0.0f;
	};

public:
	ShaderTextureTransparent(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvDescriptorSize, std::wstring& filepath);
	~ShaderTextureTransparent();

	void UpdateObjectCB(DirectX::XMFLOAT4X4* itemWorldMatrix, UINT cbIndex);

	void Init() override;
	void CreatePsoAndRootSignature(VertexType vertexType, DXGI_FORMAT& rtvFormat, DXGI_FORMAT& dsvFormat) override;
	
	void BeginDraw(ID3D12GraphicsCommandList* cmdList) override;
	void Draw(ID3D12GraphicsCommandList* cmdList, IRenderer* drawnMeshR) override;
	void EndDraw(ID3D12GraphicsCommandList* cmdList) override;

	float uvOffsetY;
protected:
	std::vector<UploadBuffer<OffSetConstants>*> m_offSetCb;

};
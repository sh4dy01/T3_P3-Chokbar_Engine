#pragma once

#include "UploadBuffer.h"

enum VertexType { POS, POS_COLOR, POS_TEX, POS_NORM_TEX, POS_NORM_TEX_TAN };

class ShaderBase
{
public:
	ShaderBase(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, std::wstring& filepath);
	virtual ~ShaderBase();

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

	std::vector<UploadBuffer<ObjConstants>*> m_objectCBs;
	UploadBuffer<PassConstants>* m_passCB;
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_inputLayout;

	ID3D12RootSignature* m_rootSignature;

	ID3D10Blob* m_vsByteCode;
	ID3D10Blob* m_psByteCode;

	ID3D12PipelineState* m_pipelineState;

	ID3D12Device* m_generalDevice;
	ID3D12DescriptorHeap* m_generalCBVHeap;

	Camera* m_generalCamera;

protected:
	virtual void Init() = 0;
	virtual void CreatePsoAndRootSignature(VertexType vertexType, DXGI_FORMAT& rtvFormat, DXGI_FORMAT& dsvFormat) = 0;
	void SetInputLayout(VertexType vertexType);

	virtual void BeginDraw(ID3D12GraphicsCommandList* cmdList) = 0;
	virtual void Draw(ID3D12GraphicsCommandList* cmdList, MeshGeometry* mg) = 0;
	virtual void EndDraw(ID3D12GraphicsCommandList* cmdList) = 0;

	virtual void AddObjectCB() = 0;
	virtual void UpdateObjectCB(const float dt, const float totalTime) = 0;

	void CreatePassCB();
	void UpdatePassCB(const float dt, const float totalTime);

	void CompileShader(const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target, ID3DBlob* uploader);
};

class ShaderSimple : public ShaderBase
{
public:
	ShaderSimple(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, std::wstring& filepath);
	~ShaderSimple();

	void Init() override;
	void CreatePsoAndRootSignature(VertexType vertexType, DXGI_FORMAT& rtvFormat, DXGI_FORMAT& dsvFormat) override;

	void AddObjectCB() override;
	void UpdateObjectCB(const float dt, const float totalTime) override;

	void BeginDraw(ID3D12GraphicsCommandList* cmdList) override;
	void Draw(ID3D12GraphicsCommandList* cmdList, MeshGeometry* mg) override;
	void EndDraw(ID3D12GraphicsCommandList* cmdList) override;

private:

};
#include "Chokbar.h"

#include "D3D/Geometry/D3DMesh.h"
#include "Texture.h"
#include "D3D/Renderers/MeshRenderer.h"
#include "D3D/Renderers/ParticleRenderer.h"
#include "D3D/Base/D3DRenderer.h"

#include "ShaderBase.h"

using namespace DirectX;

#pragma region SHADER BASE
ShaderBase::ShaderBase(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, const UINT cbvDescriptorSize, const std::wstring& filepath)
	: m_filepath(filepath), m_generalDevice(device), m_cbvDescriptorSize(cbvDescriptorSize)
{
	m_passCB = nullptr;

	m_rootSignature = nullptr;
	m_pipelineState = nullptr;

	m_vsByteCode = nullptr;
	m_psByteCode = nullptr;
}

ShaderBase::~ShaderBase()
{
	NULLPTR(m_generalDevice)

	RELPTR(m_pipelineState)
	RELPTR(m_rootSignature)

	RELPTR(m_vsByteCode)
	RELPTR(m_psByteCode)

	for (auto& cb : m_objectCBs)
		DELPTR(cb)

	m_objectCBs.clear();
	DELPTR(m_passCB)
}

void ShaderBase::Init()
{
	CreatePassCB();

	CompileShader(nullptr, "vs_main", "vs_5_1", &m_vsByteCode);
	CompileShader(nullptr, "ps_main", "ps_5_1", &m_psByteCode);
}

void ShaderBase::SetInputLayout(VertexType vertexType)
{
	// Input Layout represents the data we are sending to the shader via the VS_INPUT struct.
	m_inputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	m_inputLayout.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	m_inputLayout.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	m_inputLayout.push_back({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	m_inputLayout.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 52, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
}

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 2> ShaderBase::GetStaticSamplers()
{
	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW


	return { pointWrap, linearWrap };
}

void ShaderBase::UnBind(const UINT index)
{
	DELPTR(m_objectCBs[index])
	m_freeIndices.push(index);
}

ShaderBase* ShaderBase::Bind()
{
	AddObjectCB();
	return this;
}

void ShaderBase::AddObjectCB() 
{ 
	auto ub = new UploadBuffer<ObjConstants>(m_generalDevice, 1, true);
	m_objectCBs.push_back(ub);
	NULLPTR(ub)
}

void ShaderBase::UpdateObjectCB(DirectX::XMFLOAT4X4* itemWorldMatrix, UINT cbIndex)
{
	if (cbIndex >= m_objectCBs.size())
		AddObjectCB();

	ObjConstants objConstants;
	objConstants.World = *itemWorldMatrix;
	m_objectCBs[cbIndex]->CopyData(0, &objConstants);
}

void ShaderBase::CreatePassCB() { m_passCB = new UploadBuffer<PassConstants>(m_generalDevice, 1, true); }

void ShaderBase::UpdatePassCB(const float dt, const float totalTime) const
{
	const XMMATRIX camView = CameraManager::GetMainCamera()->GetView();
	const XMMATRIX camOrthoProj = CameraManager::GetMainCamera()->GetOrthoProj();
	const XMMATRIX camViewProj = CameraManager::GetMainCamera()->GetViewProj();

	PassConstants mainPassCB;

	XMStoreFloat4x4(&mainPassCB.View, XMMatrixTranspose(camView));
	XMStoreFloat4x4(&mainPassCB.OrthoProj, XMMatrixTranspose(camOrthoProj));
	XMStoreFloat4x4(&mainPassCB.ViewProj, XMMatrixTranspose(camViewProj));

	mainPassCB.LightColor = XMFLOAT4(0.9f, 0.7f, 0.7f, 1.0f);
	mainPassCB.LightDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	mainPassCB.EyePosW = CameraManager::GetMainCamera()->transform->m_pParent->GetPosition();
	mainPassCB.TotalTime = totalTime;
	mainPassCB.DeltaTime = dt;

	m_passCB->CopyData(0, &mainPassCB);
}

void ShaderBase::CompileShader(const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target, ID3DBlob** uploader) const
{
	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	
	ID3DBlob* errors = nullptr;

	const HRESULT hr =
		D3DCompileFromFile(m_filepath.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, entrypoint.c_str(), target.c_str(), compileFlags, 0, uploader, &errors);
	ThrowIfFailed(hr)
	
	if (errors != nullptr)
	{
		MessageBoxA(0, (char*)errors->GetBufferPointer(), 0, 0);
		RELPTR(errors)
	}
	ThrowIfFailed(hr)
}
#pragma endregion 

#pragma region SHADER SIMPLE
ShaderSimple::ShaderSimple(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvDescriptorSize, std::wstring& filepath)
	: ShaderBase(device, cbvHeap, cbvDescriptorSize, filepath)
{
}

ShaderSimple::~ShaderSimple()
= default;

void ShaderSimple::Init()
{
	ShaderBase::Init();
}

void ShaderSimple::CreatePsoAndRootSignature(VertexType vertexType, DXGI_FORMAT& rtvFormat, DXGI_FORMAT& dsvFormat)
{
	SetInputLayout(vertexType);

	// We create our root parameters here. They are used to pass data to our shaders.
	// We have 2 root parameters here, one for our object constant buffer and one for our pass constant buffer.
	// InitAsConstantBufferView() takes the register index of the constant buffer we want to pass to our shader.
	// I.e : first parameter will be registered as b0 in our shader (b stands for buffer, because we use the InitAsConstantBufferView() method).
	CD3DX12_ROOT_PARAMETER slotRootParameter[2];
	slotRootParameter[0].InitAsConstantBufferView(0);
	slotRootParameter[1].InitAsConstantBufferView(1);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(_countof(slotRootParameter), slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* serializedRootSignature = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSignature, &errorBlob);
	ThrowIfFailed(hr)
	hr = m_generalDevice->CreateRootSignature(0, serializedRootSignature->GetBufferPointer(), serializedRootSignature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	ThrowIfFailed(hr)

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { m_inputLayout.data(), static_cast<UINT>(m_inputLayout.size()) };
	psoDesc.pRootSignature = m_rootSignature;
	psoDesc.VS = { reinterpret_cast<BYTE*>(m_vsByteCode->GetBufferPointer()), m_vsByteCode->GetBufferSize() };
	psoDesc.PS = { reinterpret_cast<BYTE*>(m_psByteCode->GetBufferPointer()), m_psByteCode->GetBufferSize() };
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = rtvFormat;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;
	psoDesc.DSVFormat = dsvFormat;

	hr = m_generalDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
	ThrowIfFailed(hr)

	RELPTR(serializedRootSignature)
	RELPTR(errorBlob)
}

void ShaderSimple::BeginDraw(ID3D12GraphicsCommandList* cmdList)
{
	// Bind the root signature to the command list
	cmdList->SetGraphicsRootSignature(m_rootSignature);

	// Bind the pass constant buffer to the pipeline at b1
	cmdList->SetGraphicsRootConstantBufferView(1, m_passCB->GetResource()->GetGPUVirtualAddress());

	// Bind the pipeline state to the command list
	cmdList->SetPipelineState(m_pipelineState);

	// Set the primitive topology
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ShaderSimple::Draw(ID3D12GraphicsCommandList* cmdList, IRenderer* drawnMeshR)
{
	if (drawnMeshR->ObjectCBIndex >= m_objectCBs.size())
		AddObjectCB();

	assert(drawnMeshR->ObjectCBIndex <= m_objectCBs.size());

	cmdList->IASetVertexBuffers(0, 1, &drawnMeshR->Mesh->VertexBufferView());
	cmdList->IASetIndexBuffer(&drawnMeshR->Mesh->IndexBufferView());

	cmdList->SetGraphicsRootConstantBufferView(0, m_objectCBs[drawnMeshR->ObjectCBIndex]->GetResource()->GetGPUVirtualAddress());

	cmdList->DrawIndexedInstanced(drawnMeshR->Mesh->GetIndexCount(), 1, 0, 0, 0);
}

void ShaderSimple::EndDraw(ID3D12GraphicsCommandList* cmdList)
{

}
#pragma endregion

#pragma region SHADER TEXTURE
ShaderTexture::ShaderTexture(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvDescriptorSize, std::wstring& filepath)
	: ShaderBase(device, cbvHeap, cbvDescriptorSize, filepath)
{
}

ShaderTexture::~ShaderTexture()
= default;

void ShaderTexture::Init()
{
	ShaderBase::Init();
}

void ShaderTexture::CreatePsoAndRootSignature(VertexType vertexType, DXGI_FORMAT& rtvFormat, DXGI_FORMAT& dsvFormat)
{
	SetInputLayout(vertexType);

	// Since we need to pass a texture to our shader, we need to create a descriptor table.
	// We set its register to 0, meaning it would be accessible via the t0 register in our shader.
	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// Unlike the simple shader, we have 3 root parameters here.
	// The first parameter represent our texture given to the shader as a descriptor table.
	CD3DX12_ROOT_PARAMETER slotRootParameter[3];
	slotRootParameter[0].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);
	slotRootParameter[1].InitAsConstantBufferView(0);
	slotRootParameter[2].InitAsConstantBufferView(1);

	// Get static samplers that can be passed to the shader directly by the root signature.
	// Non static samplers must be passed to the shader via shader resources.
	auto samplers = GetStaticSamplers();

	// Create our root signature description.
	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(_countof(slotRootParameter), slotRootParameter, samplers.size(), samplers.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* serializedRootSignature = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSignature, &errorBlob);
	ThrowIfFailed(hr)
	hr = m_generalDevice->CreateRootSignature(0, serializedRootSignature->GetBufferPointer(), serializedRootSignature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	ThrowIfFailed(hr)

	if (errorBlob != nullptr)
	{
		// This error catching will be opening a pop up with any error inside the HLSL code.
		MessageBoxA(nullptr, static_cast<char*>(errorBlob->GetBufferPointer()), nullptr, 0);
		RELPTR(errorBlob)
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { m_inputLayout.data(), static_cast<UINT>(m_inputLayout.size()) };
	psoDesc.pRootSignature = m_rootSignature;
	psoDesc.VS = { reinterpret_cast<BYTE*>(m_vsByteCode->GetBufferPointer()), m_vsByteCode->GetBufferSize() };
	psoDesc.PS = { reinterpret_cast<BYTE*>(m_psByteCode->GetBufferPointer()), m_psByteCode->GetBufferSize() };
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = rtvFormat;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;
	psoDesc.DSVFormat = dsvFormat;

	hr = m_generalDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
	ThrowIfFailed(hr)

	RELPTR(serializedRootSignature)
}

void ShaderTexture::BeginDraw(ID3D12GraphicsCommandList* cmdList)
{
	cmdList->SetGraphicsRootSignature(m_rootSignature);

	cmdList->SetGraphicsRootConstantBufferView(2, m_passCB->GetResource()->GetGPUVirtualAddress());

	cmdList->SetPipelineState(m_pipelineState);

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ShaderTexture::Draw(ID3D12GraphicsCommandList* cmdList, IRenderer* drawnMeshR)
{
	// UINT are not supposed to be negative, it will only be negative if it is not initialized (debug only).
	// ReSharper disable once CppUnsignedZeroComparison
	assert(drawnMeshR->GetTexture(0)->HeapIndex >= 0);

	// If the object has no constant buffer index, we add one.
	// This is not supposed to happen, but it is a security.
	if (drawnMeshR->ObjectCBIndex >= m_objectCBs.size())
		AddObjectCB();

	assert(drawnMeshR->ObjectCBIndex <= m_objectCBs.size());

	cmdList->IASetVertexBuffers(0, 1, &drawnMeshR->Mesh->VertexBufferView());
	cmdList->IASetIndexBuffer(&drawnMeshR->Mesh->IndexBufferView());

	auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(I(D3DRenderer)->GetCbvHeap()->GetGPUDescriptorHandleForHeapStart());
	cbvHandle.Offset(drawnMeshR->GetTexture(0)->HeapIndex, m_cbvDescriptorSize);

	cmdList->SetGraphicsRootDescriptorTable(0, cbvHandle);
	cmdList->SetGraphicsRootConstantBufferView(1, m_objectCBs[drawnMeshR->ObjectCBIndex]->GetResource()->GetGPUVirtualAddress());
	
	cmdList->DrawIndexedInstanced(drawnMeshR->Mesh->GetIndexCount(), 1, 0, 0, 0);
}

void ShaderTexture::EndDraw(ID3D12GraphicsCommandList* cmdList)
{
}
#pragma endregion

#pragma region SHADER PARTICLE
ShaderParticle::ShaderParticle(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvDescriptorSize, std::wstring& filepath)
	: ShaderBase(device, cbvHeap, cbvDescriptorSize, filepath)
{
	m_particleInstanceDataBuffer = nullptr;
}

ShaderParticle::~ShaderParticle()
{
	DELPTR(m_particleInstanceDataBuffer);
}

void ShaderParticle::Init()
{
	ShaderBase::Init();

	m_particleInstanceDataBuffer = new UploadBuffer<InstanceData>(m_generalDevice, MAX_PARTICLE_COUNT, false);
}

void ShaderParticle::CreatePsoAndRootSignature(VertexType vertexType, DXGI_FORMAT& rtvFormat, DXGI_FORMAT& dsvFormat)
{
	SetInputLayout(vertexType);

	CD3DX12_ROOT_PARAMETER slotRootParameter[2];
	slotRootParameter[0].InitAsShaderResourceView(0, 1);
	slotRootParameter[1].InitAsConstantBufferView(0);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(_countof(slotRootParameter), slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* serializedRootSignature = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSignature, &errorBlob);
	ThrowIfFailed(hr)
	hr = m_generalDevice->CreateRootSignature(0, serializedRootSignature->GetBufferPointer(), serializedRootSignature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	ThrowIfFailed(hr)

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { m_inputLayout.data(), (UINT)m_inputLayout.size() };
	psoDesc.pRootSignature = m_rootSignature;
	psoDesc.VS = { reinterpret_cast<BYTE*>(m_vsByteCode->GetBufferPointer()), m_vsByteCode->GetBufferSize() };
	psoDesc.PS = { reinterpret_cast<BYTE*>(m_psByteCode->GetBufferPointer()), m_psByteCode->GetBufferSize() };
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	// We need to enable blending for our particles so we use alpha blending.
	D3D12_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	psoDesc.BlendState = blendDesc;

	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = rtvFormat;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;
	psoDesc.DSVFormat = dsvFormat;

	hr = m_generalDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
	ThrowIfFailed(hr);

	RELPTR(serializedRootSignature);
	RELPTR(errorBlob);
}

void ShaderParticle::BeginDraw(ID3D12GraphicsCommandList* cmdList)
{
	cmdList->SetGraphicsRootSignature(m_rootSignature);

	cmdList->SetGraphicsRootConstantBufferView(1, m_passCB->GetResource()->GetGPUVirtualAddress());

	cmdList->SetPipelineState(m_pipelineState);

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ShaderParticle::Draw(ID3D12GraphicsCommandList* cmdList, IRenderer* drawnMeshR)
{
	auto* pr = dynamic_cast<ParticleRenderer*>(drawnMeshR);
	assert(pr);
	DrawAsParticle(cmdList, pr);
}

void ShaderParticle::DrawAsParticle(ID3D12GraphicsCommandList* cmdList, const ParticleRenderer* drawnMeshR)
{
	if (drawnMeshR->ObjectCBIndex >= m_objectCBs.size())
		AddObjectCB();

	assert(drawnMeshR->ObjectCBIndex <= m_objectCBs.size());

	cmdList->IASetVertexBuffers(0, 1, &drawnMeshR->Mesh->VertexBufferView());
	cmdList->IASetIndexBuffer(&drawnMeshR->Mesh->IndexBufferView());

	auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(I(D3DRenderer)->GetCbvHeap()->GetGPUDescriptorHandleForHeapStart());
	cbvHandle.Offset(drawnMeshR->ObjectCBIndex, m_cbvDescriptorSize);

	cmdList->SetGraphicsRootShaderResourceView(0, m_particleInstanceDataBuffer->GetResource()->GetGPUVirtualAddress());

	cmdList->DrawIndexedInstanced(drawnMeshR->Mesh->GetIndexCount(), drawnMeshR->GetParticleCount(), 0, 0, 0);
}

void ShaderParticle::EndDraw(ID3D12GraphicsCommandList* cmdList)
{

}

void ShaderParticle::UpdateParticleInstanceDataBuffer(const int startIndex, const void* data) const
{
	m_particleInstanceDataBuffer->CopyData(startIndex, data);
}
#pragma endregion

#pragma region SHADER SKYBOX
ShaderSkybox::ShaderSkybox(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvDescriptorSize, std::wstring& filepath)
	: ShaderTexture(device, cbvHeap, cbvDescriptorSize, filepath)
{
}

ShaderSkybox::~ShaderSkybox()
= default;

void ShaderSkybox::CreatePsoAndRootSignature(VertexType vertexType, DXGI_FORMAT& rtvFormat, DXGI_FORMAT& dsvFormat)
{
	SetInputLayout(vertexType);

	CD3DX12_DESCRIPTOR_RANGE texTable;
	texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER slotRootParameter[3];
	slotRootParameter[0].InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);
	slotRootParameter[1].InitAsConstantBufferView(0);
	slotRootParameter[2].InitAsConstantBufferView(1);

	auto samplers = GetStaticSamplers();

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(_countof(slotRootParameter), slotRootParameter, samplers.size(), samplers.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* serializedRootSignature = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSignature, &errorBlob);
	ThrowIfFailed(hr)
	hr = m_generalDevice->CreateRootSignature(0, serializedRootSignature->GetBufferPointer(), serializedRootSignature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	ThrowIfFailed(hr)

	if (errorBlob != nullptr)
	{
		MessageBoxA(nullptr, static_cast<char*>(errorBlob->GetBufferPointer()), nullptr, 0);
		RELPTR(errorBlob)
	}

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { m_inputLayout.data(), static_cast<UINT>(m_inputLayout.size()) };
	psoDesc.pRootSignature = m_rootSignature;
	psoDesc.VS = { reinterpret_cast<BYTE*>(m_vsByteCode->GetBufferPointer()), m_vsByteCode->GetBufferSize() };
	psoDesc.PS = { reinterpret_cast<BYTE*>(m_psByteCode->GetBufferPointer()), m_psByteCode->GetBufferSize() };
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = rtvFormat;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;
	psoDesc.DSVFormat = dsvFormat;

	hr = m_generalDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
	ThrowIfFailed(hr)

	RELPTR(serializedRootSignature)
}
#pragma endregion

#pragma region SHADER TEXTURE OFFSET
ShaderTextureUI::ShaderTextureUI(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, const UINT cbvDescriptorSize, std::wstring& filepath)
	: ShaderTexture(device, cbvHeap, cbvDescriptorSize, filepath)
{
}

ShaderTextureUI::~ShaderTextureUI()
{
	for (auto& cb : m_offSetCb)
		DELPTR(cb)
}

void ShaderTextureUI::Draw(ID3D12GraphicsCommandList* cmdList, IRenderer* drawnMeshR)
{
	// UINT are not supposed to be negative, it will only be negative if it is not initialized (debug only).
	// ReSharper disable once CppUnsignedZeroComparison
	assert(drawnMeshR->GetTexture(0)->HeapIndex >= 0);

	if (drawnMeshR->ObjectCBIndex >= m_offSetCb.size())
		AddObjectCB();

	assert(drawnMeshR->ObjectCBIndex <= m_offSetCb.size());

	cmdList->IASetVertexBuffers(0, 1, &drawnMeshR->Mesh->VertexBufferView());
	cmdList->IASetIndexBuffer(&drawnMeshR->Mesh->IndexBufferView());

	auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(I(D3DRenderer)->GetCbvHeap()->GetGPUDescriptorHandleForHeapStart());
	cbvHandle.Offset(drawnMeshR->GetTexture(0)->HeapIndex, m_cbvDescriptorSize);

	cmdList->SetGraphicsRootDescriptorTable(0, cbvHandle);
	cmdList->SetGraphicsRootConstantBufferView(1, m_offSetCb[drawnMeshR->ObjectCBIndex]->GetResource()->GetGPUVirtualAddress());


	cmdList->DrawIndexedInstanced(drawnMeshR->Mesh->GetIndexCount(), 1, 0, 0, 0);
}

void ShaderTextureUI::AddObjectCB()
{
	auto ub = new UploadBuffer<OffSetConstants>(m_generalDevice, 1, true);
	m_offSetCb.push_back(ub);
	NULLPTR(ub)

	ShaderBase::AddObjectCB();
}

void ShaderTextureUI::UpdateAsOffset(const DirectX::XMFLOAT4X4* itemWorldMatrix, const UINT cbIndex, const float offSetY)
{
	if (cbIndex >= m_offSetCb.size())
		AddObjectCB();

	OffSetConstants offSetConstants;
	offSetConstants.World = *itemWorldMatrix;
	offSetConstants.UVOffsetY = offSetY;
	m_offSetCb[cbIndex]->CopyData(0, &offSetConstants);
}

#pragma endregion
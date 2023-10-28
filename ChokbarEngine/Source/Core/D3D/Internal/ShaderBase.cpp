#include "Chokbar.h"

#include "Core/DebugUtils.h"
#include "Texture.h"
#include "Core/D3D/D3DUtils.h"

#include "ShaderBase.h"

using namespace DirectX;

#pragma region SHADER BASE
ShaderBase::ShaderBase(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvDescriptorSize, std::wstring& filepath)
	: m_generalDevice(device), m_generalCBVHeap(cbvHeap), m_cbvDescriptorSize(cbvDescriptorSize), m_filepath(filepath),
	Type(ShaderType::BASE)
{
	m_passCB = nullptr;

	m_rootSignature = nullptr;
	m_pipelineState = nullptr;

	m_vsByteCode = nullptr;
	m_psByteCode = nullptr;
}

ShaderBase::~ShaderBase()
{
	m_generalDevice = nullptr;
	m_generalCBVHeap = nullptr;

	m_pipelineState->Release();
	m_rootSignature->Release();

	m_vsByteCode->Release();
	m_psByteCode->Release();

	m_objectCBs.clear();
	m_passCB = nullptr;

	m_MainCamera = nullptr;
}

void ShaderBase::Init()
{
	CreatePassCB();
}

void ShaderBase::SetInputLayout(VertexType vertexType)
{
	switch (vertexType)
	{
	case POS_COLOR:
	{
		m_inputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_inputLayout.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(XMFLOAT3), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		break;
	}
	case POS_TEX:
	{
		m_inputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_inputLayout.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT3), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		break;
	}
	default:
		break;
	}
}

ShaderBase* ShaderBase::Bind()
{
	AddObjectCB();
	return this;
}

void ShaderBase::AddObjectCB()
{
	const UINT64 cBufferSize = (sizeof(ObjConstants) + 255) & ~255;
	const int index = (int)m_objectCBs.size();

	auto cb = new UploadBuffer<ObjConstants>(m_generalDevice, 1, true);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = cb->GetResource()->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)cBufferSize;

	CD3DX12_CPU_DESCRIPTOR_HANDLE cbvHeapHandle(m_generalCBVHeap->GetCPUDescriptorHandleForHeapStart());
	cbvHeapHandle.Offset(index + 1, m_cbvDescriptorSize);

	m_generalDevice->CreateConstantBufferView(&cbvDesc, cbvHeapHandle);
	m_objectCBs.push_back(cb);
}

void ShaderBase::UpdateObjectCB(DirectX::XMFLOAT4X4* itemWorldMatrix, UINT cbIndex)
{
	if (cbIndex >= m_objectCBs.size())
		AddObjectCB();

	ObjConstants objConstants;
	objConstants.World = itemWorldMatrix;
	m_objectCBs[cbIndex]->CopyData(0, &objConstants);
}

void ShaderBase::CreatePassCB()
{
	const UINT64 cBufferSize = (sizeof(PassConstants) + 255) & ~255;

	m_passCB = new UploadBuffer<PassConstants>(m_generalDevice, 1, true);
	D3D12_GPU_VIRTUAL_ADDRESS passCBAddress = m_passCB->GetResource()->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC passCBVDesc;
	passCBVDesc.BufferLocation = passCBAddress;
	passCBVDesc.SizeInBytes = (UINT)cBufferSize;
	m_generalDevice->CreateConstantBufferView(&passCBVDesc, m_generalCBVHeap->GetCPUDescriptorHandleForHeapStart());
}

void ShaderBase::UpdatePassCB(const float dt, const float totalTime)
{
	const XMMATRIX camView = m_MainCamera->GetCameraComponent()->GetView();
	const XMMATRIX camProj = m_MainCamera->GetCameraComponent()->GetProj();

	XMMATRIX viewProj = XMMatrixMultiply(camView, camProj);
	// XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(camView), camView);
	// XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(camProj), camProj);
	// XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);

	PassConstants mainPassCB;
	XMStoreFloat4x4(&mainPassCB.View, XMMatrixTranspose(camView));
	// XMStoreFloat4x4(&mainPassCB.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&mainPassCB.Proj, XMMatrixTranspose(camProj));
	// XMStoreFloat4x4(&mainPassCB.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&mainPassCB.ViewProj, XMMatrixTranspose(viewProj));
	// XMStoreFloat4x4(&mainPassCB.InvViewProj, XMMatrixTranspose(invViewProj));

	mainPassCB.EyePosW = m_MainCamera->GetCameraComponent()->transform->GetPosition();
	// mainPassCB.RenderTargetSize = XMFLOAT2(m_bufferWidth, m_bufferHeight);
	// mainPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / m_bufferWidth, 1.0f / m_bufferHeight);
	// mainPassCB.NearZ = m_camera.NearZ;
	// mainPassCB.FarZ = m_camera.FarZ;
	mainPassCB.TotalTime = totalTime;
	mainPassCB.DeltaTime = dt;

	m_passCB->CopyData(0, &mainPassCB);
}

void ShaderBase::CompileShader(const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target, ID3DBlob** uploader)
{
	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = S_OK;

	ID3DBlob* errors = nullptr;

	hr = D3DCompileFromFile(m_filepath.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, entrypoint.c_str(), target.c_str(), compileFlags, 0, uploader, &errors);

	if (errors != nullptr)
	{
		std::cout << ((char*)errors->GetBufferPointer()) << std::endl;
		errors->Release();
	}
	ThrowIfFailed(hr);
}
#pragma endregion 

#pragma region SHADER SIMPLE
ShaderSimple::ShaderSimple(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvDescriptorSize, std::wstring& filepath)
	: ShaderBase(device, cbvHeap, cbvDescriptorSize, filepath)
{
}

ShaderSimple::~ShaderSimple()
{
}

void ShaderSimple::Init()
{
	ShaderBase::Init();
	CompileShader(nullptr, "vs_main", "vs_5_0", &m_vsByteCode);
	CompileShader(nullptr, "ps_main", "ps_5_0", &m_psByteCode);

	m_MainCamera = Chokbar::Engine::GetMainCamera();
}

void ShaderSimple::CreatePsoAndRootSignature(VertexType vertexType, DXGI_FORMAT& rtvFormat, DXGI_FORMAT& dsvFormat)
{
	//CD3DX12_DESCRIPTOR_RANGE cbvTable0;
	//cbvTable0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

	//CD3DX12_DESCRIPTOR_RANGE cbvTable1;
	//cbvTable1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);

	SetInputLayout(vertexType);

	CD3DX12_ROOT_PARAMETER slotRootParameter[2];
	slotRootParameter[0].InitAsConstantBufferView(0);
	slotRootParameter[1].InitAsConstantBufferView(1);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(_countof(slotRootParameter), slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* serializedRootSignature = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSignature, &errorBlob);
	m_generalDevice->CreateRootSignature(0, serializedRootSignature->GetBufferPointer(), serializedRootSignature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));

	if (errorBlob != nullptr) errorBlob->Release();
	serializedRootSignature->Release();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { m_inputLayout.data(), 2 };
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

	m_generalDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
}

void ShaderSimple::BeginDraw(ID3D12GraphicsCommandList* cmdList)
{
	cmdList->SetGraphicsRootSignature(m_rootSignature);

	cmdList->SetGraphicsRootConstantBufferView(1, m_passCB->GetResource()->GetGPUVirtualAddress());

	cmdList->SetPipelineState(m_pipelineState);

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ShaderSimple::Draw(ShaderDrawArguments& args)
{
	if (args.RenderItemCBIndex >= m_objectCBs.size())
		AddObjectCB();

	args.CmdList->IASetVertexBuffers(0, 1, &args.RenderItemGeometry->VertexBufferView());
	args.CmdList->IASetIndexBuffer(&args.RenderItemGeometry->IndexBufferView());

	auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_generalCBVHeap->GetGPUDescriptorHandleForHeapStart());
	cbvHandle.Offset(args.RenderItemCBIndex, m_cbvDescriptorSize);

	args.CmdList->SetGraphicsRootConstantBufferView(0, m_objectCBs[args.RenderItemCBIndex]->GetResource()->GetGPUVirtualAddress());

	args.CmdList->DrawIndexedInstanced(args.IndexCount, 1, args.StartIndexLocation, args.BaseVertexLocation, 0);
}

void ShaderSimple::EndDraw(ID3D12GraphicsCommandList* cmdList)
{
	
}
#pragma endregion

#pragma region SHADER TEXTURE
ShaderTexture::ShaderTexture(ID3D12Device* device, ID3D12DescriptorHeap* cbvHeap, UINT cbvDescriptorSize, std::wstring& filepath)
	: ShaderBase(device, cbvHeap, cbvDescriptorSize, filepath)
{
	m_texture = nullptr;
}

ShaderTexture::~ShaderTexture()
{
	delete m_texture;
}

void ShaderTexture::Init()
{
	ShaderBase::Init();
	CompileShader(nullptr, "vs_main", "vs_5_0", &m_vsByteCode);
	CompileShader(nullptr, "ps_main", "ps_5_0", &m_psByteCode);

	m_MainCamera = Chokbar::Engine::GetMainCamera();

}

void ShaderTexture::CreatePsoAndRootSignature(VertexType vertexType, DXGI_FORMAT& rtvFormat, DXGI_FORMAT& dsvFormat)
{
	//CD3DX12_DESCRIPTOR_RANGE cbvTable0;
	//cbvTable0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

	//CD3DX12_DESCRIPTOR_RANGE cbvTable1;
	//cbvTable1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);

	SetInputLayout(vertexType);

	CD3DX12_ROOT_PARAMETER slotRootParameter[2];
	slotRootParameter[0].InitAsConstantBufferView(0);
	slotRootParameter[1].InitAsConstantBufferView(1);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(_countof(slotRootParameter), slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* serializedRootSignature = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSignature, &errorBlob);
	m_generalDevice->CreateRootSignature(0, serializedRootSignature->GetBufferPointer(), serializedRootSignature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));

	if (errorBlob != nullptr) errorBlob->Release();
	serializedRootSignature->Release();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { m_inputLayout.data(), 2 };
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

	m_generalDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
}

void ShaderTexture::BeginDraw(ID3D12GraphicsCommandList* cmdList)
{
	cmdList->SetGraphicsRootSignature(m_rootSignature);

	cmdList->SetGraphicsRootConstantBufferView(1, m_passCB->GetResource()->GetGPUVirtualAddress());

	cmdList->SetPipelineState(m_pipelineState);

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ShaderTexture::Draw(ShaderDrawArguments& args) 
{ 
	//TODO heck if texture is nullptr
	if (args.RenderItemCBIndex >= m_objectCBs.size())
		AddObjectCB();

	args.CmdList->IASetVertexBuffers(0, 1, &args.RenderItemGeometry->VertexBufferView());
	args.CmdList->IASetIndexBuffer(&args.RenderItemGeometry->IndexBufferView());

	auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_generalCBVHeap->GetGPUDescriptorHandleForHeapStart());
	cbvHandle.Offset(args.RenderItemCBIndex, m_cbvDescriptorSize);

	// args.CmdList->SetGraphicsRootConstantBufferView(0, m_objectCBs[args.RenderItemCBIndex]->GetResource()->GetGPUVirtualAddress());

	args.CmdList->DrawIndexedInstanced(args.IndexCount, 1, args.StartIndexLocation, args.BaseVertexLocation, 0);
}

void ShaderTexture::EndDraw(ID3D12GraphicsCommandList* cmdList)
{
}
#pragma endregion
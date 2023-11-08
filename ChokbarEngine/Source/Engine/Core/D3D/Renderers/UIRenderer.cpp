#include "Chokbar.h"

#include "Engine/ECS/Components/Component.h"
#include "D3D/Shaders/Material.h"
#include "D3D/Shaders/ShaderBase.h"

#include "UIRenderer.h"

using namespace DirectX;

UIRenderer::UIRenderer() : IRenderer()
{
}

UIRenderer::~UIRenderer()
{
}

void UIRenderer::Render(ID3D12GraphicsCommandList* cmdList)
{
	if (!IsEnabled() || !Mat || !Mesh) return;

	auto shader = Mat->GetShader();
	shader->BeginDraw(cmdList);

	shader->Draw(cmdList, this);

	shader->EndDraw(cmdList);

}

void UIRenderer::Update(float dt)
{
	if (!IsEnabled() || !Mat || !Mesh) return;

	if (transform->IsDirty())
		transform->UpdateWorldMatrix();

	Mat->GetShader()->UpdateObjectCB(transform->GetWorldMatrix(), ObjectCBIndex);
}

void UIRenderer::OffsetUV(float offsetY)
{
	auto* shader = Mat->GetShader();
	auto* shaderOffset = dynamic_cast<ShaderTextureOffset*>(shader);
	assert(shaderOffset);

	shaderOffset->SetUVOffsetY(offsetY);
}

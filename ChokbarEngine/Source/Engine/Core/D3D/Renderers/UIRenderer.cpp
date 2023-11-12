#include "Chokbar.h"

#include "Engine/ECS/Components/Component.h"
#include "D3D/Shaders/Material.h"
#include "D3D/Shaders/ShaderBase.h"

#include "UIRenderer.h"

using namespace DirectX;

UIRenderer::UIRenderer() : IRenderer(), m_uvOffsetY(0)
{
}

UIRenderer::~UIRenderer()
= default;

void UIRenderer::SetOffsetY(float offsetY)
{
	m_uvOffsetY = offsetY;
	UpdateShader();
}

void UIRenderer::AddOffsetY(float offsetY)
{
	SetOffsetY(m_uvOffsetY + offsetY);
}

void UIRenderer::UpdateShader() const
{
	if (!IsEnabled() || !Mat || !Mesh) return;

	if (const auto offsetShader = dynamic_cast<ShaderTextureUI*>(Mat->GetShader()))
	{
		transform->UpdateParentedWorldMatrix();
		offsetShader->UpdateAsOffset(transform->GetTransposedParentedWorldMatrix(), ObjectCBIndex, m_uvOffsetY);
	}
}

void UIRenderer::Render(ID3D12GraphicsCommandList* cmdList)
{
	if (!IsEnabled() || !Mat || !Mesh) return;

	const auto shader = Mat->GetShader();
	shader->BeginDraw(cmdList);

	shader->Draw(cmdList, this);

	shader->EndDraw(cmdList);

}

void UIRenderer::Update(float dt)
{
	if (!IsEnabled() || !Mat || !Mesh) return;

	transform->UpdateParentedWorldMatrix();

	Mat->GetShader()->UpdateObjectCB(transform->GetTransposedParentedWorldMatrix(), ObjectCBIndex);
}

#pragma once 
#include "Engine/ECS/Components/Component.h"
#include "IRenderer.h"

/*
UIRenderer is a component that is used to render UI elements such as score, health, etc.
Be careful to use shaders that supports UI rendering, such as ShaderTextureOffset.
*/
class UIRenderer final : public Component, public IRenderer
{
	friend class D3DRenderer;

public:
	UIRenderer();
	~UIRenderer() override;

	[[nodiscard]] float GetUVOffsetY() const { return m_uvOffsetY; }
	void SetOffsetY(float offsetY);
	void AddOffsetY(float offsetY);

private:
	void Render(ID3D12GraphicsCommandList* cmdList) override;
	void Update(float dt) override;

	void UpdateShader() const;

	float m_uvOffsetY;
};

#pragma once 
#include "Engine/ECS/Components/Component.h"
#include "IRenderer.h"

/*
UIRenderer is a component that is used to render UI elements such as score, health, etc.
Be carefull to use shaders that supports UI rendering, such as ShaderTextureOffset.
*/
class UIRenderer : public Component, public IRenderer
{
	friend class D3DRenderer;

public:
	UIRenderer();
	~UIRenderer() override;

	void OffsetUV(float offsetY);

private:
	void Render(ID3D12GraphicsCommandList* cmdList) override;
	void Update(float dt) override;
};

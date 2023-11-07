#pragma once 
#include "Engine/ECS/Components/Component.h"
#include "IRenderer.h"

// Double inheritance is not a good idea, I know.
class MeshRenderer : public Component, public IRenderer
{
public:
	MeshRenderer();
	~MeshRenderer() override;

public:
	void Render(ID3D12GraphicsCommandList* cmdList) override;
	void Update(float dt) override;
};
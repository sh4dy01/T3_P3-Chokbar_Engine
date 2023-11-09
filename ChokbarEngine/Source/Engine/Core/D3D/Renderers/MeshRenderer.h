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

	void SetClippable(bool value) { m_IsClippable = value; }
	bool IsClippable() { return m_IsClippable; }

private:

	bool m_IsClippable;

};

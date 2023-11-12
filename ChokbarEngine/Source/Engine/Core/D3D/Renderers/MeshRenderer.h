#pragma once 
#include "Engine/ECS/Components/Component.h"
#include "IRenderer.h"

/*
Classic mesh renderer component, used to render a mesh with a material
*/
class MeshRenderer : public Component, public IRenderer
{
	friend class D3DRenderer;

public:
	MeshRenderer();
	~MeshRenderer() override;

private:
	void Render(ID3D12GraphicsCommandList* cmdList) override;
	void Update(float dt) override;

public:
	void SetClippable(const bool value) { m_isClippable = value; }
	[[nodiscard]] bool IsClippable() const { return m_isClippable; }

private:
	// Should this mesh be clipped by the frustum or not
	bool m_isClippable;

};

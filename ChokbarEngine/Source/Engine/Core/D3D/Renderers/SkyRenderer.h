#pragma once 

/*
This class is responsible for rendering the skybox.
It is not working yet.
*/
class SkyRenderer final : public Component, public IRenderer
{
	// Only the D3DRenderer can Update and Render this class.
	friend class D3DRenderer;

public:
	SkyRenderer();
	~SkyRenderer() override;

private:
	void Render(ID3D12GraphicsCommandList* cmdList) override;
	void Update(float dt) override;
};
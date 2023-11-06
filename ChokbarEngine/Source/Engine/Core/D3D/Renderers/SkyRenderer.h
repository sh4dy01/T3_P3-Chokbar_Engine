#pragma once 

// Double inheritance is not a good idea, I know.
class SkyRenderer : public Component, public IRenderer
{
	friend class D3DRenderer;

public:
	SkyRenderer();
	~SkyRenderer() override;

private:
	void Render(ID3D12GraphicsCommandList* cmdList) override;
	void Update(float dt) override;
};
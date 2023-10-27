#pragma once

class ShaderBase;

class Material
{
public:
	Material();
	~Material();

	std::string Name = "";

	void AddTexture(Texture* text);

	void SetShader(ShaderBase* shader);
	ShaderBase* GetShader() const { return m_shader; }
	Texture* GetTexture(UINT index) const { return m_textures[index]; }

private:
	ShaderBase* m_shader = nullptr;
	std::vector<Texture*> m_textures{};
};
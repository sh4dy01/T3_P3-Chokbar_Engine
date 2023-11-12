#pragma once

/*
Special texture type for cubemaps textures.
*/
class Cubemap final : public Texture
{
public:
	explicit Cubemap(const std::string& name);
	~Cubemap() override;
};


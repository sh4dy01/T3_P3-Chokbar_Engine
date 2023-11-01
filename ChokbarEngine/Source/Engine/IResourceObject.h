#pragma once 

class IResourceObject
{
public:
	IResourceObject(std::string name) : m_filepath(""), m_name(name) {}
	virtual ~IResourceObject();

	virtual void Load(const std::string& filepath) = 0;

protected:
	std::string m_filepath;
	std::string m_name;
};
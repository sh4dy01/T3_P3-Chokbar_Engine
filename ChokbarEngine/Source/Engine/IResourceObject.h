#pragma once 

class IResourceObject
{
public:
	IResourceObject(const std::wstring& filepath) : m_filepath(filepath) {}
	virtual ~IResourceObject();

	virtual void Load(const std::wstring& filepath) = 0;

protected:
	const std::wstring& m_filepath;
};
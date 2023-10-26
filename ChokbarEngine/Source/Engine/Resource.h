#pragma once

#include "Engine/IResourceObject.h"

template <class T = IResourceObject>
class Resource
{
public:
	Resource() = default;
	~Resource()
	{
		m_resource.clear();
	}

	template <class T = IResourceObject>
	static T* Load<T>(const std::wstring& filepath)
	{
		T* resource = new T(filepath);
		resource->Load();
		return resource;
	}

private:
	std::vector<T*> m_resource;
};
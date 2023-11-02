#pragma once

#include "TypeDef.h"

class Transform;

class Object
{
public:

	Object();
	Object(const std::string& name);
	~Object();

	void Destroy() const;

public:

	std::string GetName() { return m_Name; }
	void SetName(const std::string& name) { m_Name = name; }

	InstanceID GetInstanceID() const { return m_InstanceID; }

protected:

	std::string m_Name;
	InstanceID m_InstanceID;
};


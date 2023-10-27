#pragma once

#include <DirectXMath.h>

#include "TypeDef.h"
#include "Engine/ECS/Components/TransformComponent.h"


class Object
{
public:

	Object();
	Object(const std::string& name);
	~Object();

	static Object Instantiate(Object original);
	//static Object Instantiate(GameObject original, Transform parent);
	//static Object Instantiate(Object original, Transform parent, bool instantiateInWorldSpace);
	static Object Instantiate(const Object& original, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, Transform parent);

	void Destroy() const;

public:

	std::string GetName() { return m_Name; }
	void SetName(const std::string& name) { m_Name = name; }

protected:

	std::string m_Name;
	InstanceID m_InstanceID;
};


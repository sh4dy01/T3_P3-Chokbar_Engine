#pragma once 
#include "Engine/ECS/Components/Component.h"
#include "Engine/ECS/Components/Collision/TriggerCollisionEvent.h"

#include <DirectXMath.h>

class Collider : public Component
{
public:

	enum class ShapeType {
		Sphere,
	};

public:

	~Collider() override;

	void CallOnTriggerEnter(Collider* other) const;
	void CallOnTriggerStay(Collider* other) const;
	void CallOnTriggerExit(Collider* other) const;

	void OnAddedComponent() override;
	void RegisterTriggerCollisionEvent(TriggerCollisionEvent* triggerCollisionEvent);


	void SetCenter(const DirectX::XMFLOAT3& center) { m_Center = center; };
	DirectX::XMFLOAT3 GetCenter() const { return m_Center; };

	ShapeType GetType() const { return m_type; };

	int GetGridSize() { return m_GridSize; };


protected:

	Collider();
	void SetGridSize(int gridPos) { m_GridSize = gridPos; };

protected:

	ShapeType m_type;

	DirectX::XMFLOAT3 m_Center;

	int m_GridSize;

private:

	std::vector<TriggerCollisionEvent*> m_triggerCollisionEvents;

};
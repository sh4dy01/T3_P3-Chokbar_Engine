#pragma once 
#include "Engine/ECS/Components/Component.h"
#include "Engine/ECS/Components/Collision/TriggerCollisionEvent.h"


class Collider : public Component
{
public:

    enum class ShapeType {
		Sphere,
	};

public:

	~Collider() override;

	void CallOnTriggerEnter(Collider* other);
	void CallOnTriggerStay(Collider* other);
	void CallOnTriggerExit(Collider* other);

	void OnAddedComponent() override;
	void RegisterTriggerCollisionEvent(TriggerCollisionEvent* triggerCollisionEvent);


	void SetCenter(const DirectX::XMFLOAT3& center) { m_Center = center; };
	DirectX::XMFLOAT3 GetCenter() const { return m_Center; };

	ShapeType GetType() const { return m_type; };

protected:

	Collider();

protected:

	ShapeType m_type;

	DirectX::XMFLOAT3 m_Center;

private:

	std::vector<TriggerCollisionEvent*> m_triggerCollisionEvents;

};
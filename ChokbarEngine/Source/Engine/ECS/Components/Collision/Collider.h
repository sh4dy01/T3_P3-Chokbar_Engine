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

	Collider();
	~Collider() override;

	void CallOnTriggerEnter(Collider* other);
	void CallOnTriggerStay(Collider* other);
	void CallOnTriggerExit(Collider* other);

	void OnAddedComponent() override;
	void RegisterTriggerCollisionEvent(TriggerCollisionEvent* triggerCollisionEvent);

    //virtual bool CheckCollision(const CollisionShape& other) const = 0;

	DirectX::XMFLOAT3 GetCenter() const { return m_Center; };
	ShapeType GetType() const { return m_type; };

	void SetOffset(const DirectX::XMFLOAT3& offset) { m_Center = offset; };


protected:

	ShapeType m_type;

	DirectX::XMFLOAT3 m_Center;

private:

	std::vector<TriggerCollisionEvent*> m_triggerCollisionEvents;

};
#pragma once 
#include "Engine/ECS/Components/Component.h"
#include "Engine/ECS/Components/Collision/TriggerCollisionEvent.h"


class CollisionShape : public Component
{
public:

    enum class ShapeType {
		Sphere,
	};

public:

	CollisionShape();
	virtual ~CollisionShape() = 0;

	void CallOnTriggerEnter(CollisionShape* other);
	void CallOnTriggerStay(CollisionShape* other);
	void CallOnTriggerExit(CollisionShape* other);

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
#pragma once 
#include "Engine/ECS/Components/Component.h"


using namespace Chokbar;

class CollisionShape : public Component
{
public:

    enum class ShapeType {
		Sphere,
	};

public:

	CollisionShape();
	virtual ~CollisionShape() = default;

	void OnAddedComponent() override;

    //virtual bool CheckCollision(const CollisionShape& other) const = 0;

	ShapeType GetType() const;


protected:

	ShapeType m_type;
};
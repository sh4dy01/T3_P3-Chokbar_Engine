#include "BlankProject.h"
#include "Projectile.h"

#include "Scripts/ProjectileBehavior.h"

Projectile::Projectile()
{
	AddComponent<MeshRenderer>()->Init(SPHERE, SIMPLE);
	AddComponent<Rigidbody>();
	AddComponent<SphereCollider>();

	AddComponent<ProjectileBehavior>();

	transform->SetScale(0.1f);
}

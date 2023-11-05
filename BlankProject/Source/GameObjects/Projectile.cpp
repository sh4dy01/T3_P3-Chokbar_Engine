#include "BlankProject.h"
#include "Projectile.h"

#include "Scripts/ProjectileBehavior.h"

Projectile::Projectile()
{
	AddComponent<MeshRenderer>(new MeshRenderer())->Init(SPHERE, SIMPLE);
	AddComponent<Rigidbody>();
	AddComponent<SphereCollider>();

	AddComponent<ProjectileBehavior>();
}

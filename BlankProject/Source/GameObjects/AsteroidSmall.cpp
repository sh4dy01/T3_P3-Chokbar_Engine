#include "AsteroidSmall.h"

#include "Scripts/Asteroids/AsteroidSmallBehaviour.h"


AsteroidSmall::AsteroidSmall()
{
   AddComponent<MeshRenderer>()->Init(SPHERE, SIMPLE);
   AddComponent<Rigidbody>();
   AddComponent<SphereCollider>();
   AddComponent<AsteroidSmallBehaviour>();

   transform->SetScale(1.0f);

}

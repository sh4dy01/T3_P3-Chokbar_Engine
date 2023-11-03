#include "Chokbar.h"

#include "Particle.h"

#include <cmath>
using namespace DirectX;


Particle::Particle()
	: m_LifeTime(0.0f), m_CurrentLifeTime(0.0f),m_Velocity(XMFLOAT3(0.0f, 0.0f, 0.0f)), m_AngularVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f)), m_IsActive(false)
{
	m_Color1 = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	m_Color2 = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

Particle::~Particle()
{
}

void Particle::Update(float deltaTime)
{
	m_CurrentLifeTime -= deltaTime;

	/*
	_position += _direction * (_speed * Time.deltaTime);
	Vector3 rotation = _rotation.eulerAngles;
	rotation.x += 10 * Time.deltaTime;
	rotation.y += 10 * Time.deltaTime;
	rotation.z += 10 * Time.deltaTime;
	_rotation = Quaternion.Euler(rotation);


	_particleMaterial.color = Color.Lerp(_color1, _color2, particle.CurrentLifeTime / particle.LifeTime);
	particle.Render(_particleMesh, _particleMaterial);
	_particleMaterial.color = _color1;
	*/
}

void Particle::Reset()
{
	m_CurrentLifeTime = 0.0f;
	m_AngularVelocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Velocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}


void Particle::Init(float rLifeTime, DirectX::XMFLOAT3 rVel, DirectX::XMFLOAT3 rAngVel)
{
	m_LifeTime = rLifeTime;
	m_CurrentLifeTime = 0.0f;

	m_AngularVelocity = rAngVel;
	m_Velocity = rVel;

	m_IsActive = true;
}
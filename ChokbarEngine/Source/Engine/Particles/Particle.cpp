#include "Chokbar.h"

#include "Particle.h"

#include <cmath>


Particle::Particle(const float lifeTime, float speed, DirectX::XMFLOAT3 dir)
	: m_LifeTime(lifeTime), m_CurrentLifeTime(0.0f), m_Speed(speed), m_Direction(dir), m_IsActive(false), m_opacity(1.0f)
{
	m_Color1 = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	m_Color2 = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

Particle::~Particle()
{
}

void Particle::Update(float deltaTime)
{
	float ratio = m_CurrentLifeTime / m_LifeTime;
	m_opacity = std::lerp(0, 1, ratio);

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
	m_Direction = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Speed = 0.0f;

	//TODO reset position & rotation
}


void Particle::Start()
{
	m_IsActive = true;
}
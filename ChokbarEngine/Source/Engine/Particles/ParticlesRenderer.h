#pragma once

#define MAX_PARTICLES 100

class Particle;
struct Vertex;

class ParticlesRenderer
{
public:
	ParticlesRenderer();
	~ParticlesRenderer();

public:
	void Awake();
	void Start();
	void Update(float deltaTime);

	void CreateGlobalMesh(std::array<Particle*, MAX_PARTICLES>& ar);

private:
	std::array<Vertex*, MAX_PARTICLES * 8> m_ParticleVertices;
	std::array<UINT*, MAX_PARTICLES * 36> m_ParticleIndices;

	MeshRenderer* m_MeshRenderer;

	void RecreateGlobalMeshGPU();

};
#pragma once 
#include <DirectXMath.h>

class GeometryGenerator
{
	public:
	struct Vertex
	{
		Vertex() {}
		Vertex(
			const DirectX::XMFLOAT3& position,
			const DirectX::XMFLOAT3& normal,
			const DirectX::XMFLOAT3& tangent,
			const DirectX::XMFLOAT2& uv
		);
		Vertex(
			float px, float py, float pz,
			float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v
		);
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 TangentU;
		DirectX::XMFLOAT2 TexC;
	};

	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<UINT> Indices32;

		std::vector<UINT16>& GetIndices16();

	private:
		std::vector<UINT16> m_Indices16;
	};

	MeshData CreatePyramid(float width, float height, float depth);
};

struct Transform
{
	DirectX::XMFLOAT3 Position = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 Rotation = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 Scale = { 1.0f, 1.0f, 1.0f };
};
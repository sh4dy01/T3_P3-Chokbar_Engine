#include "Chokbar.h"
#include "D3Dmath.h"

GeometryGenerator::Vertex::Vertex(
	const XMFLOAT3& position,
	const XMFLOAT3& normal,
	const XMFLOAT3& tangent,
	const XMFLOAT2& uv
) :
	Position(position),
	Normal(normal),
	TangentU(tangent),
	TexC(uv)
{
}

GeometryGenerator::Vertex::Vertex(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty, float tz, float u, float v) :
	Position(px, py, pz),
	Normal(nx, ny, nz),
	TangentU(tx, ty, tz),
	TexC(u, v)
{
}

std::vector<UINT16>& GeometryGenerator::MeshData::GetIndices16()
{
	if (m_Indices16.empty())
	{
		m_Indices16.resize(Indices32.size());
		for (size_t i = 0; i < Indices32.size(); ++i)
			m_Indices16[i] = static_cast<UINT16>(Indices32[i]);
	}

	return m_Indices16;
}

GeometryGenerator::MeshData GeometryGenerator::CreatePyramid(float width, float height, float depth)
{
	MeshData meshData;

	

	return meshData;
}

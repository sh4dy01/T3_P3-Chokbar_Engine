#include "Chokbar.h"
#include "D3Dmath.h"

using namespace DirectX;

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



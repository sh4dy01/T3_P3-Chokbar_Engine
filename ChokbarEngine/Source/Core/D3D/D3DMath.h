#pragma once 
#include <DirectXMath.h>
#include <vector>

#include "Internal/D3DMesh.h"


class GeometryGenerator
{
	public:
	

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
#pragma once 
#include <DirectXMath.h>
#include <vector>

#include "Internal/D3DMesh.h"
#include "Core/D3D/MeshType.h"



static class GeometryHandler
{
	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<UINT> Indices16;
	};

	struct MeshData_Color
	{
		std::vector<Vertex_Color> Vertices;
		std::vector<UINT> Indices16;
	};

	struct MeshData_UV
	{
		std::vector<Vertex_UV> Vertices;
		std::vector<UINT> Indices16;
	};

public:

	static void CreateAllMeshes();

	static D3DMesh* GetMesh(MeshType type) { return m_Meshes[type]; }

private:

	static D3DMesh* CreatePyramid();

private:

	static D3DMesh* m_Meshes[4];
};
#include "Chokbar.h"
#include "D3Dmath.h"
#include <DirectXColors.h>

using namespace DirectX;

D3DMesh GeometryHandler::m_Meshes[4];

void GeometryHandler::CreateAllMeshes()
{
	m_Meshes[3] = CreatePyramid();
}

D3DMesh GeometryHandler::CreatePyramid()
{
	MeshData meshData;

	Vertex vList[] =
	{
		{ XMFLOAT3(0.0f, 1.0f, 0.0f),  Colors::White, XMFLOAT2(0, 0)},
		{ XMFLOAT3(-0.5f, 0.0f, -0.5f), Colors::White, XMFLOAT2(0, 0) },
		{ XMFLOAT3(-0.5f, 0.0f, 0.5f),  Colors::White, XMFLOAT2(0, 0) },
		{ XMFLOAT3(0.5f, 0.0f, 0.5f),   Colors::White, XMFLOAT2(0, 0) },
		{ XMFLOAT3(0.5f, 0.0f, -0.5f),  Colors::White, XMFLOAT2(0, 0) },
	};

	for (auto v : vList)
	{
		meshData.Vertices.push_back(v);
	}


	UINT16 iList[] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1 };

	for (auto i: iList)
	{
		meshData.Indices16.push_back(i);
	}

	D3DMesh mesh;
	mesh.Create(meshData.Vertices, meshData.Indices16);

	return mesh;
}



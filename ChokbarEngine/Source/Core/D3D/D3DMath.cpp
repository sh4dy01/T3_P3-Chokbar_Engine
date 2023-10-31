#include "Chokbar.h"
#include "D3Dmath.h"
#include <DirectXColors.h>

using namespace DirectX;

D3DMesh* GeometryHandler::m_Meshes[4];

void GeometryHandler::CreateAllMeshes()
{
	m_Meshes[0] = CreateCube();
	m_Meshes[3] = CreatePyramid();
}

D3DMesh* GeometryHandler::CreateCube()
{
	Vertex_UV vList[] = {
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) }, // Vertex 0
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) }, // Vertex 1
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) }, // Vertex 2
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) }, // Vertex 3
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT2(0.0f, 1.0f) }, // Vertex 4
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT2(0.0f, 0.0f) }, // Vertex 5
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT2(1.0f, 0.0f) }, // Vertex 6
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT2(1.0f, 1.0f) }, // Vertex 7
	};

	UINT iList[] = {
		0, 1, 2, 0, 2, 3,

		4, 6, 5, 4, 7, 6,

		4, 5, 1, 4, 1, 0,

		3, 2, 6, 3, 6, 7,

		1, 5, 6, 1, 6, 2,

		4, 0, 3, 4, 3, 7
	};

	D3DMesh* mesh = new D3DMesh();
	mesh->Create(vList, sizeof(Vertex_UV), _countof(vList), iList, sizeof(UINT), _countof(iList));

	return mesh;
}

D3DMesh* GeometryHandler::CreatePyramid()
{
	Vertex_Color vList[] =
	{
		{ XMFLOAT3(0.0f, 1.0f, 0.0f),   Colors::Red       },
		{ XMFLOAT3(-0.5f, 0.0f, -0.5f), Colors::White     },
		{ XMFLOAT3(-0.5f, 0.0f, 0.5f),  Colors::LightBlue },
		{ XMFLOAT3(0.5f, 0.0f, 0.5f),   Colors::White     },
		{ XMFLOAT3(0.5f, 0.0f, -0.5f),  Colors::White     },
	};


	UINT iList[] = { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1 };

	D3DMesh* mesh = new D3DMesh();
	mesh->Create(vList, sizeof(Vertex_Color), _countof(vList), iList, sizeof(UINT), _countof(iList));

	return mesh;
}



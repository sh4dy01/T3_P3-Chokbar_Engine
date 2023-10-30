#include "Chokbar.h"
#include "D3Dmath.h"
#include <DirectXColors.h>

using namespace DirectX;

D3DMesh* GeometryHandler::m_Meshes[4];

void GeometryHandler::CreateAllMeshes()
{
	m_Meshes[3] = CreatePyramid();
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



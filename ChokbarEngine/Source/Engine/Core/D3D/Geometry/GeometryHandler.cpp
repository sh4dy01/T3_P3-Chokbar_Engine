#include "Chokbar.h"

#include "Engine/Core/D3D/Base/MeshType.h"

#include "GeometryHandler.h"

using namespace DirectX;

D3DMesh* GeometryHandler::m_Meshes[4];

void GeometryHandler::CreateAllMeshes()
{
	m_Meshes[0] = CreateCube();
	m_Meshes[1] = CreateGeoSphere();
	m_Meshes[2] = CreatePyramid();
	m_Meshes[3] = CreateSquare();
}

void GeometryHandler::DestroyAllMeshes()
{
	for (int i = 0; i < _countof(m_Meshes); i++)
	{
		DELPTR(m_Meshes[i]);
	}
}

D3DMesh* GeometryHandler::CreateCube()
{
	Vertex v[24];

	float width = 0.5f;
	float height = 0.5f;
	float depth = 0.5f;

	// Fill in the front face vertex data.
	v[0] = Vertex(-width, -height, -depth, 0.0f, 0.0f, 0.0f, 1.0F, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-width, +height, -depth, 0.0f, 0.0f, 0.0f, 1.0F, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(+width, +height, -depth, 0.0f, 0.0f, 0.0f, 1.0F, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = Vertex(+width, -height, -depth, 0.0f, 0.0f, 0.0f, 1.0F, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = Vertex(-width, -height, +depth, 0.0f, 0.0f, 0.0f, 1.0F, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = Vertex(+width, -height, +depth, 0.0f, 0.0f, 0.0f, 1.0F, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = Vertex(+width, +height, +depth, 0.0f, 0.0f, 0.0f, 1.0F, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = Vertex(-width, +height, +depth, 0.0f, 0.0f, 0.0f, 1.0F, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8] = Vertex(-width, +height, -depth, 0.0f, 0.0f, 0.0f, 1.0F, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9] = Vertex(-width, +height, +depth, 0.0f, 0.0f, 0.0f, 1.0F, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = Vertex(+width, +height, +depth, 0.0f, 0.0f, 0.0f, 1.0F, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = Vertex(+width, +height, -depth, 0.0f, 0.0f, 0.0f, 1.0F, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = Vertex(-width, -height, -depth, 0.0f, 0.0f, 0.0f, 1.0F, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = Vertex(+width, -height, -depth, 0.0f, 0.0f, 0.0f, 1.0F, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = Vertex(+width, -height, +depth, 0.0f, 0.0f, 0.0f, 1.0F, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = Vertex(-width, -height, +depth, 0.0f, 0.0f, 0.0f, 1.0F, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = Vertex(-width, -height, +depth, 0.0f, 0.0f, 0.0f, 1.0F, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = Vertex(-width, +height, +depth, 0.0f, 0.0f, 0.0f, 1.0F, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = Vertex(-width, +height, -depth, 0.0f, 0.0f, 0.0f, 1.0F, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = Vertex(-width, -height, -depth, 0.0f, 0.0f, 0.0f, 1.0F, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = Vertex(+width, -height, -depth, 0.0f, 0.0f, 0.0f, 1.0F, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = Vertex(+width, +height, -depth, 0.0f, 0.0f, 0.0f, 1.0F, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = Vertex(+width, +height, +depth, 0.0f, 0.0f, 0.0f, 1.0F, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = Vertex(+width, -height, +depth, 0.0f, 0.0f, 0.0f, 1.0F, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	UINT i[36] =
	{
		0, 1,  2,  0,  2,  3,

		4, 5,  6,  4,  6,  7,

		8, 9,  10, 8,  10, 11,

		12, 13, 14, 12, 14, 15,

		16, 17, 18, 16, 18, 19,

		20, 21, 22, 20, 22, 23
	};

	D3DMesh* mesh = NEW D3DMesh();
	mesh->Create(v, sizeof(Vertex), _countof(v), i, sizeof(UINT), _countof(i));

	return mesh;
}

D3DMesh* GeometryHandler::CreateUVSphere()
{
	const float radius = 1.0F;
	const UINT slices = 20;
	const UINT stacks = 20;

	std::vector<Vertex> vertices;
	std::vector<UINT> indices;

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	Vertex topVertex(0.0f, +radius, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Vertex bottomVertex(0.0f, -radius, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	vertices.push_back(topVertex);

	float phiStep = XM_PI / stacks;
	float thetaStep = 2.0f * XM_PI / slices;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= stacks - 1; ++i)
	{
		float phi = i * phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= slices; ++j)
		{
			float theta = j * thetaStep;

			Vertex v;

			// spherical to cartesian
			v.Position.x = radius * sinf(phi) * cosf(theta);
			v.Position.y = radius * cosf(phi);
			v.Position.z = radius * sinf(phi) * sinf(theta);

			v.Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			// Partial derivative of P with respect to theta
			v.TangentU.x = -radius * sinf(phi) * sinf(theta);
			v.TangentU.y = 0.0f;
			v.TangentU.z = +radius * sinf(phi) * cosf(theta);

			XMVECTOR T = XMLoadFloat3(&v.TangentU);
			XMStoreFloat3(&v.TangentU, XMVector3Normalize(T));

			XMVECTOR p = XMLoadFloat3(&v.Position);
			XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

			v.TexC.x = theta / XM_2PI;
			v.TexC.y = phi / XM_PI;

			vertices.push_back(v);
		}
	}

	vertices.push_back(bottomVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (UINT i = 1; i <= slices; ++i)
	{
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = slices + 1;
	for (UINT i = 0; i < stacks - 2; ++i)
	{
		for (UINT j = 0; j < slices; ++j)
		{
			indices.push_back(baseIndex + i * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < slices; ++i)
	{
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}

	D3DMesh* mesh = NEW D3DMesh();
	mesh->Create(vertices.data(), sizeof(Vertex), (UINT)vertices.size(), indices.data(), sizeof(UINT), (UINT)indices.size());

	return mesh;
}

D3DMesh* GeometryHandler::CreateGeoSphere()
{
	const float radius = 1;
	const UINT subDivisions = 6;

	std::vector<Vertex> vertices;
	std::vector<UINT> indices;

	// Approximate a sphere by tessellating an icosahedron.

	const float X = 0.525731f;
	const float Z = 0.850651f;

	const UINT numVertices = 12;
	XMFLOAT3 pos[numVertices] =
	{
		XMFLOAT3(-X, 0.0f, Z),  XMFLOAT3(X, 0.0f, Z),
		XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),
		XMFLOAT3(0.0f, Z, X),   XMFLOAT3(0.0f, Z, -X),
		XMFLOAT3(0.0f, -Z, X),  XMFLOAT3(0.0f, -Z, -X),
		XMFLOAT3(Z, X, 0.0f),   XMFLOAT3(-Z, X, 0.0f),
		XMFLOAT3(Z, -X, 0.0f),  XMFLOAT3(-Z, -X, 0.0f)
	};

	UINT k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	vertices.resize(numVertices);
	indices.assign(&k[0], &k[60]);

	for (UINT i = 0; i < numVertices; ++i)
		vertices[i].Position = pos[i];

	for (UINT i = 0; i < subDivisions; ++i)
		Subdivide(vertices, indices);

	// Project vertices onto sphere and scale.
	for (UINT i = 0; i < vertices.size(); ++i)
	{
		// Project onto unit sphere.
		XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&vertices[i].Position));

		// Project onto sphere.
		XMVECTOR p = radius * n;

		XMStoreFloat3(&vertices[i].Position, p);
		XMStoreFloat3(&vertices[i].Normal, n);

		// Derive texture coordinates from spherical coordinates.
		float theta = atan2f(vertices[i].Position.z, vertices[i].Position.x);

		// Put in [0, 2pi].
		if (theta < 0.0f)
			theta += XM_2PI;

		float phi = acosf(vertices[i].Position.y / radius);

		vertices[i].TexC.x = theta / XM_2PI;
		vertices[i].TexC.y = phi / XM_PI;

		// Partial derivative of P with respect to theta
		vertices[i].TangentU.x = -radius * sinf(phi) * sinf(theta);
		vertices[i].TangentU.y = 0.0f;
		vertices[i].TangentU.z = +radius * sinf(phi) * cosf(theta);

		XMVECTOR T = XMLoadFloat3(&vertices[i].TangentU);
		XMStoreFloat3(&vertices[i].TangentU, XMVector3Normalize(T));
	}

	D3DMesh* mesh = NEW D3DMesh();
	mesh->Create(vertices.data(), sizeof(Vertex), (UINT)vertices.size(), indices.data(), sizeof(UINT), (UINT)indices.size());

	return mesh;
}

void GeometryHandler::Subdivide(std::vector<Vertex>& vertices, std::vector<UINT>& indices)
{
	// Save a copy of the input geometry.
	std::vector<Vertex> copyVertices = vertices;
	std::vector<UINT> copyIndices = indices;


	vertices.resize(0);
	indices.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	UINT numTris = (UINT)copyIndices.size() / 3;
	for (UINT i = 0; i < numTris; ++i)
	{
		Vertex v0 = copyVertices[copyIndices[i * 3 + 0]];
		Vertex v1 = copyVertices[copyIndices[i * 3 + 1]];
		Vertex v2 = copyVertices[copyIndices[i * 3 + 2]];

		//
		// Generate the midpoints.
		//

		Vertex m0 = MidPoint(v0, v1);
		Vertex m1 = MidPoint(v1, v2);
		Vertex m2 = MidPoint(v0, v2);

		//
		// Add new geometry.
		//

		vertices.push_back(v0); // 0
		vertices.push_back(v1); // 1
		vertices.push_back(v2); // 2
		vertices.push_back(m0); // 3
		vertices.push_back(m1); // 4
		vertices.push_back(m2); // 5

		indices.push_back(i * 6 + 0);
		indices.push_back(i * 6 + 3);
		indices.push_back(i * 6 + 5);

		indices.push_back(i * 6 + 3);
		indices.push_back(i * 6 + 4);
		indices.push_back(i * 6 + 5);

		indices.push_back(i * 6 + 5);
		indices.push_back(i * 6 + 4);
		indices.push_back(i * 6 + 2);

		indices.push_back(i * 6 + 3);
		indices.push_back(i * 6 + 1);
		indices.push_back(i * 6 + 4);
	}
}

Vertex GeometryHandler::MidPoint(const Vertex& v0, const Vertex& v1)
{
	XMVECTOR p0 = XMLoadFloat3(&v0.Position);
	XMVECTOR p1 = XMLoadFloat3(&v1.Position);

	XMVECTOR n0 = XMLoadFloat3(&v0.Normal);
	XMVECTOR n1 = XMLoadFloat3(&v1.Normal);

	XMVECTOR tan0 = XMLoadFloat3(&v0.TangentU);
	XMVECTOR tan1 = XMLoadFloat3(&v1.TangentU);

	XMVECTOR tex0 = XMLoadFloat2(&v0.TexC);
	XMVECTOR tex1 = XMLoadFloat2(&v1.TexC);

	// Compute the midpoints of all the attributes.  Vectors need to be normalized
	// since linear interpolating can make them not unit length.  
	XMVECTOR pos = 0.5f * (p0 + p1);
	XMVECTOR normal = XMVector3Normalize(0.5f * (n0 + n1));
	XMVECTOR tangent = XMVector3Normalize(0.5f * (tan0 + tan1));
	XMVECTOR tex = 0.5f * (tex0 + tex1);

	Vertex v;
	XMStoreFloat3(&v.Position, pos);
	XMStoreFloat3(&v.Normal, normal);
	XMStoreFloat3(&v.TangentU, tangent);
	XMStoreFloat2(&v.TexC, tex);

	return v;
}

D3DMesh* GeometryHandler::CreatePyramid()
{
	Vertex vList[18];

	vList[0] = Vertex(XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(0.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.5f, 0.0f));
	vList[1] = Vertex(XMFLOAT3(-0.5f, 0.0f, -0.5f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(0.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));
	vList[2] = Vertex(XMFLOAT3(-0.5f, 0.0f, 0.5f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(0.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));

	vList[3] = Vertex(XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.5f, 0.0f));
	vList[4] = Vertex(XMFLOAT3(-0.5f, 0.0f, 0.5f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));
	vList[5] = Vertex(XMFLOAT3(0.5f, 0.0f, 0.5f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));

	vList[6] = Vertex(XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.5f, 0.0f));
	vList[7] = Vertex(XMFLOAT3(0.5f, 0.0f, 0.5f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));
	vList[8] = Vertex(XMFLOAT3(0.5f, 0.0f, -0.5f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));

	vList[9] = Vertex(XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.5f, 0.0f));
	vList[10] = Vertex(XMFLOAT3(0.5f, 0.0f, -0.5f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));
	vList[11] = Vertex(XMFLOAT3(-0.5f, 0.0f, -0.5f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));

	vList[12] = Vertex(XMFLOAT3(-0.5f, 0.0f, -0.5f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.5f, 0.0f));
	vList[13] = Vertex(XMFLOAT3(-0.5f, 0.0f, 0.5f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));
	vList[14] = Vertex(XMFLOAT3(0.5f, 0.0f, 0.5f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));

	vList[15] = Vertex(XMFLOAT3(0.5f, 0.0f, 0.5f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(0.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.5f, 0.0f));
	vList[16] = Vertex(XMFLOAT3(0.5f, 0.0f, -0.5f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(0.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));
	vList[17] = Vertex(XMFLOAT3(-0.5f, 0.0f, -0.5f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(0.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));

	/*Vertex vList[] =
	{
		{ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.8f, 0.1f, 0.1f, 1.0f), XMFLOAT3(0.0f, 1.0f, -1.0f),   },
		{ XMFLOAT3(-0.5f, 0.0f, -0.5f), Colors::White },
		{ XMFLOAT3(-0.5f, 0.0f, 0.5f),  Colors::LightBlue },
		{ XMFLOAT3(0.5f, 0.0f, 0.5f),   Colors::White },
		{ XMFLOAT3(0.5f, 0.0f, -0.5f),  Colors::White },
	};*/


	UINT iList[] =
	{
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,

		12, 13, 14,
		15, 16, 17
	};

	D3DMesh* mesh = NEW D3DMesh();
	mesh->Create(vList, sizeof(Vertex), _countof(vList), iList, sizeof(UINT), _countof(iList));

	return mesh;
}

D3DMesh* GeometryHandler::CreateSquare()
{
	Vertex vList[4];

	float width = 0.5f;
	float height = 0.5f;
	float depth = 0.5f;

	vList[0] = Vertex(
		XMFLOAT3(-width, height, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT3(0.0f, 0.0f, 1.0f),
		XMFLOAT3(1.0f, 0.0f, 0.0f),
		XMFLOAT2(0.0f, 0.0f)
	);

	vList[1] = Vertex(
		XMFLOAT3(-width, -height, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT3(0.0f, 0.0f, 1.0f),
		XMFLOAT3(1.0f, 0.0f, 0.0f),
		XMFLOAT2(0.0f, 0.1f)
	);

	vList[2] = Vertex(
		XMFLOAT3(width, height, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT3(0.0f, 0.0f, 1.0f),
		XMFLOAT3(1.0f, 0.0f, 0.0f),
		XMFLOAT2(1.0f, 0.0f)
	);

	vList[3] = Vertex(
		XMFLOAT3(width, -height, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT3(0.0f, 0.0f, 1.0f),
		XMFLOAT3(1.0f, 0.0f, 0.0f),
		XMFLOAT2(1.0f, 0.1f)
	);

	UINT iList[] =
	{
		0, 2, 1,
		2, 3, 1
	};

	D3DMesh* mesh = new D3DMesh();
	mesh->Create(vList, sizeof(Vertex), _countof(vList), iList, sizeof(UINT), _countof(iList));

	return mesh;
}
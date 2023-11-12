#pragma once 

#include "D3DMesh.h"

/*
Static class that handles the creation and destruction of all meshes used in the engine.
This meshes are only used as a buffer, each Renderer uses the meh without modifying it.
This saves us a lot of memory to prevent creating the same mesh multiple times.
*/
class GeometryHandler
{
public:

	static void CreateAllMeshes();
	static void DestroyAllMeshes();

	static D3DMesh* GetMesh(const MeshType type) { return m_meshes[type]; }

private:

	static D3DMesh* CreateCube();

	static D3DMesh* CreateUVSphere();
	static D3DMesh* CreateGeoSphere();
	static void Subdivide(std::vector<Vertex>& vertices, std::vector<UINT>& indices);
	static Vertex MidPoint(const Vertex& v0, const Vertex& v1);

	static D3DMesh* CreatePyramid();

	static D3DMesh* CreateUISquare(const float uvOffsetX, const float uvOffsetY);

private:

	static D3DMesh* m_meshes[5];
};
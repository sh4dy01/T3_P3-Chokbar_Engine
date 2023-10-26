#pragma once

#include "D3DMath.h"
#include "D3DUtils.h"

#include "ShaderBase.h"

struct RenderItem
{
	RenderItem() = default;

	Transform Transform;
	DirectX::XMFLOAT4X4 World = Identity4x4();

	UINT ObjCBIndex = -1;

	MeshGeometry* Geo = nullptr;
	ShaderBase* Shader = nullptr;
	Texture* Texture = nullptr;

	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	INT BaseVertexLocation = 0;

	TRANSFORMATION_TYPE TransformationType = TRANSLATION;
};
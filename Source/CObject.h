#pragma once
#include <d3d9.h>
#include "CMesh.h"

class CObject
{

//vars
public:

//funcs
public:
	CObject();
	CObject(float x, float y, float z, float rx, float ry, float rz);
	virtual ~CObject();
	virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice) { return S_OK; }

//vars
protected:
	CMesh* mpMesh;
	D3DXMATRIXA16 mWorldTransform;
	float mSpeed;

//funcs
protected:
	void GetWorldTransform(float x, float y, float z, float rx, float ry, float rz);
};
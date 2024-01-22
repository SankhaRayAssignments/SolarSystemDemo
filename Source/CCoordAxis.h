#pragma once
#include "CMesh.h"

class CCoordAxis : public CMesh
{

//funcs
public:
	virtual ~CCoordAxis();
	virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice);
	virtual HRESULT Construct(LPDIRECT3DDEVICE9 pd3dDevice);

};

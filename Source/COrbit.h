#pragma once
#include "cmesh.h"

#define NUM_DIVISION 2
#define TOTLONG 360.0f

class COrbit :
	public CMesh
{
public:
	COrbit(float distance);
	~COrbit(void);

	//override in derived classes to render strips, lists etc.
	HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice) ;

	//override in derived classes to construct according to desired geometry
	HRESULT Construct(LPDIRECT3DDEVICE9 pd3dDevice); 

private:
	float mDistance;


};

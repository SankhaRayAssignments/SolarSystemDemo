#include "DXUT.h"
#include "CCoordAxis.h"

#define NUMCAINDICES 6

// DTOR
CCoordAxis::~CCoordAxis()
{
}

HRESULT CCoordAxis::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr = S_OK;

	V_RETURN(DrawIndexedLineList(pd3dDevice,0,0,0));

	return hr;
}

HRESULT CCoordAxis::Construct(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr = S_OK;

	V_RETURN(CreateVertexBuffer(pd3dDevice, sizeof(SPosCol), 6, D3DFVF_XYZ | D3DFVF_NORMAL, 
		D3DUSAGE_WRITEONLY, D3DPOOL_MANAGED));
	
	SPosCol* pVdata = NULL;

	V_RETURN(LockVB(0, (void**)&pVdata));
	// x
	*pVdata = SPosCol(0, 0, 0, D3DCOLOR_ARGB(255,255,0,0)); pVdata++; 
	*pVdata = SPosCol(10, 0, 0, D3DCOLOR_ARGB(255,255,0,0)); pVdata++; 
	// y
	*pVdata = SPosCol(0, 0, 0, D3DCOLOR_ARGB(255,0,255,0)); pVdata++; 
	*pVdata = SPosCol(0, 10, 0, D3DCOLOR_ARGB(255,0,255,0)); pVdata++; 
	// z
	*pVdata = SPosCol(0, 0, 0, D3DCOLOR_ARGB(255, 255,255,255)); pVdata++; 
	*pVdata = SPosCol(0, 0, 10, D3DCOLOR_ARGB(255,255,255,255)); 

	V_RETURN(UnLockVB());

	V_RETURN(CreateIndexBuffer(pd3dDevice, sizeof(USHORT), NUMCAINDICES, D3DFMT_INDEX16,
		D3DUSAGE_WRITEONLY, D3DPOOL_MANAGED));

	USHORT* pIData = NULL;

	USHORT caIndices[NUMCAINDICES] = { 0,1,2,3,4,5 };

	V_RETURN(LockIB(0, (void**)&pIData));

	memcpy(pIData,caIndices, sizeof(USHORT)*NUMCAINDICES);

	V_RETURN(UnLockIB());

	return hr;
}
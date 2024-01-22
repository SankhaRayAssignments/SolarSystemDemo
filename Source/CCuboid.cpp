#include "DXUT.h"
#include "CCuboid.h"

//#define NUMCUBEINDICES 21		//	Using Less Cube Indices can be done
#define NUMCUBEINDICES 20

CCuboid::CCuboid(D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	mMin = min;
	mMax = max;
}

CCuboid::~CCuboid()
{
}

HRESULT CCuboid::Construct(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr = S_OK;

	mWidth = mMax.x - mMin.x;
	mHeight = mMax.y - mMin.y;
	mDepth = mMax.z - mMin.z;
	
	V_RETURN(CreateVertexBuffer(pd3dDevice, sizeof(SPosCol), 8, D3DFVF_XYZ | D3DFVF_DIFFUSE, 
		D3DUSAGE_WRITEONLY, D3DPOOL_MANAGED));
	
	
	SPosCol* pVdata = NULL;

	V_RETURN(LockVB(0, (void**)&pVdata));


	*pVdata = SPosCol(mMin.x, mMin.y, mMin.z, D3DCOLOR_ARGB(125,0,0,0)); pVdata++; 
	*pVdata = SPosCol(mMax.x, mMin.y, mMin.z, D3DCOLOR_ARGB(120,0,0,255)); pVdata++; 
	*pVdata = SPosCol(mMax.x, mMax.y, mMin.z, D3DCOLOR_ARGB(120,0,255,0)); pVdata++; 
	*pVdata = SPosCol(mMin.x, mMax.y, mMin.z, D3DCOLOR_ARGB(120,0,255,255)); pVdata++; 

	*pVdata = SPosCol(mMin.x, mMin.y, mMax.z, D3DCOLOR_ARGB(120,255,0,0)); pVdata++; 
	*pVdata = SPosCol(mMax.x, mMin.y, mMax.z, D3DCOLOR_ARGB(120,255,0,255)); pVdata++; 
	*pVdata = SPosCol(mMax.x, mMax.y, mMax.z, D3DCOLOR_ARGB(120,255,255,0)); pVdata++; 
	*pVdata = SPosCol(mMin.x, mMax.y, mMax.z, D3DCOLOR_ARGB(120,255,255,255)); 


	V_RETURN(UnLockVB());

	V_RETURN(CreateIndexBuffer(pd3dDevice, sizeof(USHORT), NUMCUBEINDICES, D3DFMT_INDEX16,
		D3DUSAGE_WRITEONLY, D3DPOOL_MANAGED));

	USHORT* pIData = NULL;

	USHORT cubeIndices[NUMCUBEINDICES] = {	3,0,7,4,6,
								5,2,1,3,0,
								0,1,4,5,5,
								6,6,2,7,3
							
							//	4				Not Needed Left Side Not Visible
								};

	V_RETURN(LockIB(0, (void**)&pIData));

	memcpy(pIData,cubeIndices, sizeof(USHORT)*NUMCUBEINDICES);

	V_RETURN(UnLockIB());

	return hr;
}

HRESULT CCuboid::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr = S_OK;

	V_RETURN(DrawIndexedTriStrip(pd3dDevice,0,0,0));

	return hr;
}
/*
void CCuboid::SetMinMax(D3DXVECTOR3& minIn, D3DXVECTOR3& maxIn) 
	{ 
		mMin = minIn;
		mMax = maxIn; 
	}

*/
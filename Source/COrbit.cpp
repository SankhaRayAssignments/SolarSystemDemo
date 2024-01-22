#include "dxut.h"
#include "COrbit.h"

COrbit::COrbit(float distance)
{
	mDistance = distance ;
}

COrbit::~COrbit(void)
{
}

HRESULT COrbit::Render(LPDIRECT3DDEVICE9 pd3dDevice) 
{
	HRESULT hr;

	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE); 

	SetFVFVBIBOnDevice( pd3dDevice );
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE);
	V_RETURN( pd3dDevice->DrawIndexedPrimitive ( D3DPT_LINELIST, 0, 0, mNumVertices, 0, mNumVertices/2)  );
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE);

	return hr;
}


HRESULT COrbit::Construct(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr;

	mNumVertices = (int)TOTLONG/NUM_DIVISION ;
	SPosCol *vertices = (SPosCol *)malloc ( mNumVertices *sizeof ( SPosCol ) );

	float dx,dy,dz;
	for ( int theta = 0, index=0; theta < (int)TOTLONG; theta += NUM_DIVISION, index++)
	{
		dx = /*OriginX + */cos( D3DXToRadian(theta)) * mDistance;
		dy = 0.0f;
		dz = /*OriginY + */sin( D3DXToRadian(theta)) * mDistance;
		vertices[index] = SPosCol ( dx,dy,dz, D3DCOLOR_ARGB( 255,255,255,255));
	}


	SPosCol *pVData = NULL;
	// Create Vertex Buffer
	V_RETURN ( CreateVertexBuffer ( pd3dDevice,  sizeof(SPosCol), mNumVertices, D3DFVF_XYZ | D3DFVF_DIFFUSE, 
		D3DUSAGE_WRITEONLY, D3DPOOL_MANAGED));
	// Lock
	V_RETURN ( LockVB ( 0, (void**)&pVData ) );
	// Copy Data
	memcpy(pVData, vertices, sizeof(SPosCol)*mNumVertices);
	// Unlock
	V_RETURN ( UnLockVB ());

	//free ( vertices );


	// Fill Index Buffer
	mNumIndices = mNumVertices;
	USHORT *indices = (USHORT *)malloc ( mNumIndices * sizeof (USHORT ));
	for ( int i = 0 ; i < mNumIndices; i++ )
		indices[i] = (USHORT)i ;

	USHORT *pIData	= NULL;

	V_RETURN(CreateIndexBuffer(pd3dDevice, sizeof(USHORT), mNumIndices, D3DFMT_INDEX16,
		D3DUSAGE_WRITEONLY, D3DPOOL_MANAGED));
	// Lock
	V_RETURN(LockIB(0, (void**)&pIData));
	// Copy Data
	memcpy(pIData, indices, sizeof(USHORT)*mNumIndices);
	// Unlock
	V_RETURN(UnLockIB());

	free ( indices );




	return hr;
}


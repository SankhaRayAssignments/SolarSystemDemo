#include "dxut.h"
#include "CMarker.h"

CMarker::CMarker(D3DXVECTOR3 minIn, D3DXVECTOR3 maxIn, string texName)
{
	mMin		= minIn;
	mMax		= maxIn;
	mTexName	= texName;
	mTexture	= NULL;
	D3DXMatrixIdentity ( &mWorldMatrix );
}

CMarker::~CMarker(void)
{
	SAFE_RELEASE ( mTexture );
}
void CMarker::UpdateWorldMatrix ( D3DXMATRIXA16& matIn )
{
	mWorldMatrix = matIn;
	mWorldMatrix._42 += 5;
}

//override in derived classes to construct according to desired geometry
HRESULT CMarker::Construct(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr;

	int numVerts = 15;

	SPosColTex *markerVerts = (SPosColTex *)malloc ( numVerts*sizeof ( SPosColTex ));
	int a , r ,g ,b;
	a = r = g = b = 255;
	// Create Vertex Buffer
	V_RETURN ( CreateVertexBuffer ( pd3dDevice, sizeof ( SPosColTex ), numVerts, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1,
									D3DUSAGE_WRITEONLY, D3DPOOL_MANAGED ) );
/*
	markerVerts[0] = SPosColTex ( mMin.x, mMax.y, mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 0.0, 0.0 ); 
	markerVerts[1] = SPosColTex ( mMin.x, mMin.y, mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 0.0, 1.0 ); 
	markerVerts[2] = SPosColTex ( mMax.x, mMin.y, mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 1.0, 1.0 ); 
	markerVerts[3] = SPosColTex ( mMax.x, mMax.y, mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 1.0, 0.0 ); 
*/	
	float delX = (mMax.x - mMin.x ) / 4 ;
	float delY = (mMax.y - mMin.y ) / 3 ;

	markerVerts[0] = SPosColTex ( mMin.x + 2 * delX	, mMin.y,			mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 0.5, 1 );  // Point of Arrow 0
	markerVerts[1] = SPosColTex ( mMin.x		, mMin.y + delY,	mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 0, 0.66 ); 	// Left Of Arrow 1 
	markerVerts[2] = SPosColTex ( mMin.x + delX , mMin.y + delY,	mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 0.25, 0.66 ); 	// RightSide Of Arrow 2

	markerVerts[3] = SPosColTex ( mMin.x + 2 * delX	, mMin.y,			mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 0.5, 1 );  // Point of Arrow 0
	markerVerts[4] = SPosColTex ( mMin.x + delX , mMin.y + delY,	mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 0.25, 0.66 ); 	// RightSide Of Arrow 2
	markerVerts[5] = SPosColTex ( mMin.x + delX,  mMax.y ,			mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 0.25, 0 ); // Point of Arrow 3

	markerVerts[6] = SPosColTex ( mMin.x + 2 * delX	, mMin.y,			mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 0.5, 1 );  // Point of Arrow 0
	markerVerts[7] = SPosColTex ( mMin.x + delX,  mMax.y,			mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 0.25, 0 ); // Point of Arrow 3
	markerVerts[8] = SPosColTex ( mMin.x + 3*delX, mMax.y,			mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 0.75, 0 );  // Point of Arrow 4

	markerVerts[9] = SPosColTex ( mMin.x + 2 * delX	, mMin.y,			mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 0.5, 1 );  // Point of Arrow 0
	markerVerts[10] = SPosColTex ( mMin.x + 3*delX, mMax.y,			mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 0.75, 0 );  // Point of Arrow 4
	markerVerts[11] = SPosColTex (mMin.x + 3*delX, mMin.y + delY,	mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 0.75, 0.66 ); 	// RightSide Of Arrow 5

	markerVerts[12] = SPosColTex ( mMin.x + 2 * delX	, mMin.y,			mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 0.5, 1 );  // Point of Arrow 0
	markerVerts[13] = SPosColTex (mMin.x +  3*delX, mMin.y + delY,	mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 0.75, 0.66 ); 	// RightSide Of Arrow 5
	markerVerts[14] = SPosColTex (mMax.x			 , mMin.y + delY,	mMin.z, D3DCOLOR_ARGB ( a, r, g, b), 1, 0.66 ); 	// RightSide Of Arrow 5

	SPosColTex *pVData = NULL;

	// Lock
	V_RETURN ( LockVB ( 0, (void**)&pVData ) );

	// Copy
	memcpy ( pVData, markerVerts, numVerts * sizeof ( SPosColTex) );

	//Unlock
	V_RETURN ( UnLockVB() );
	free ( markerVerts );
	TCHAR temp[200];
	mbstowcs ( temp, mTexName.data(), 200 );
	V_RETURN ( D3DXCreateTextureFromFile ( pd3dDevice, temp, &mTexture ) );

	return hr;

}
HRESULT CMarker::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT  hr;
	
	// For tex 0
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );


	// Set the texture
	V_RETURN ( pd3dDevice->SetTexture ( 0, mTexture ) );

	//Draw
//	pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE); 
	
	pd3dDevice->SetTransform( D3DTS_WORLD, &mWorldMatrix );
	
	int sv = 0;
	for ( int i = 0 ; i < 5 ; i++)
	{
		sv = ( i * 3 );
		V_RETURN(pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, sv, 1));
	}

	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW ); //D3DCULL_NONE); //

	pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);


	// Set the textures to NULL/Clear
	pd3dDevice->SetTexture ( 0, NULL );

	return hr;
}


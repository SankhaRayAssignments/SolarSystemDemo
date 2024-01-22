#include "dxut.h"
#include "CSphere.h"


CSphere::CSphere( string texName , float lat,  float Long )
{
	D3DXMatrixIdentity( &mWorldMatrix );
	mTexName = texName ;
	mRadius = 1.0f;
	mLat = lat;
	mLong = Long;
	mBangCoord = D3DXVECTOR3 (0,0,0);
}

CSphere::~CSphere(void)
{
	SAFE_RELEASE ( mTexture );
}
D3DXVECTOR3 CSphere::GetBangCoord (   )
{
	return mBangCoord;
}


HRESULT CSphere::Construct(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr;
	int numVerts = ( TOTLAT + 1 )* (TOTLONG + 1 );
	SPosNormalUV *sphereVerts = (SPosNormalUV*)malloc( sizeof(SPosNormalUV)* numVerts); 



	int index = 0;	
	float  ui = ( 1 / TOTLONG), vi= (1 / TOTLAT);

	for(int lat = -TOTLAT/2; lat <= TOTLAT/2 ; lat++)
	{	
		float rad = D3DXToRadian ( lat );
		float newRadius = mRadius * cos ( rad );
		float delY = mRadius * sin ( rad );

	
		for(int Long = 0 ; Long <= TOTLONG ; Long++)
		{
			float delX , delZ;
			delX = newRadius * cos ( D3DXToRadian ( Long ));
			delZ = newRadius * sin ( D3DXToRadian ( Long ));


			sphereVerts[ index ].x = delX;
			sphereVerts[ index].y = delY;
			sphereVerts[ index].z = delZ;
			D3DXVECTOR3 temp;
			temp = D3DXVECTOR3 ( delX, delY, delZ ) - D3DXVECTOR3 ( 0, 0, 0 );
			D3DXVec3Normalize ( &temp, &temp );
			sphereVerts[ index].Normal =  temp;
			sphereVerts[ index ].u = Long * ui;	
			sphereVerts[ index ].v = 1 - (lat + TOTLAT/2) * vi;

			if ( Long == mLong && lat == mLat )
				mBangCoord = D3DXVECTOR3 ( delX,  delY, delZ );

			index++;
		}
	//	vi++;
	}




	SPosNormalUV *pVData = NULL;



	// Create Vertex Buffer
	V_RETURN ( CreateVertexBuffer ( pd3dDevice,  sizeof(SPosNormalUV), numVerts, D3DFVF_XYZ |D3DFVF_NORMAL | D3DFVF_TEX1, 
		D3DUSAGE_WRITEONLY, D3DPOOL_MANAGED));

	// Lock
	V_RETURN ( LockVB ( 0, (void**)&pVData ) );

	// Copy Data
	memcpy(pVData, sphereVerts, sizeof(SPosNormalUV)*numVerts);

	// Unlock
	V_RETURN ( UnLockVB ());



	//////////	 INDEX BUFFER	//
	// Create Index Buffer

	int numIndices = (TOTLAT  )*((TOTLONG+1)*2 + 2);
	USHORT* sphereIndices = (USHORT*) malloc(sizeof(USHORT)*numIndices); 

	int k = 0;
	for(int i = 0; i < TOTLAT; i++)
	{
		int j;
		for(j=0; j <= TOTLONG; j++)
		{
			//add (i,j), ((i+1),j), 
			sphereIndices[k++] = (USHORT)(i*(TOTLONG + 1) + j); 
			sphereIndices[k++] = (USHORT)((i+1)*(TOTLONG+1) + j); 
		}

		//for degenerate triangles
		sphereIndices[k++] =  (USHORT)((i+1)*(TOTLONG+1) + (j-1)); 
		sphereIndices[k++] = (USHORT)((i+1)*(TOTLONG +1)+ (j-1)); 
	}


	USHORT *pIData	= NULL;

	V_RETURN(CreateIndexBuffer(pd3dDevice, sizeof(USHORT), numIndices, D3DFMT_INDEX16,
		D3DUSAGE_WRITEONLY, D3DPOOL_MANAGED));

	// Lock
	V_RETURN(LockIB(0, (void**)&pIData));




	// Copy Data
	memcpy(pIData, sphereIndices, sizeof(USHORT)*numIndices);

	// Unlock
	V_RETURN(UnLockIB());


	free(sphereVerts);
	free(sphereIndices);


	TCHAR temp[200];
	mbstowcs ( temp, mTexName.data(), 200 );
	V_RETURN ( D3DXCreateTextureFromFile ( pd3dDevice, temp, &mTexture ) );

	return hr;
}
HRESULT CSphere::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT  hr;
	
	// For tex 0
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );

	// base tex on Stage 0

	pd3dDevice->SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
	pd3dDevice->SetTextureStageState ( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE );

	// Set the texture
	pd3dDevice->SetTexture ( 0, mTexture );

	//Draw
//	pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE); //

	pd3dDevice->SetTransform( D3DTS_WORLD, &mWorldMatrix );

	V_RETURN(DrawIndexedTriStrip(pd3dDevice, 0,0,0));
	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW ); //D3DCULL_NONE); //

	pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);


	// Set the textures to NULL/Clear
	pd3dDevice->SetTexture ( 0, NULL );

	return hr;

}



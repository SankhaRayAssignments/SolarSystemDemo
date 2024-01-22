#include "dxut.h"
#include "CSkyBox.h"


CSkyBox::~CSkyBox(void)
{
	for(int i=0;i<6;i++)
	 { 
		 cube[i]->Release();
	 }
}

	
CSkyBox::CSkyBox(D3DXVECTOR3 mMin ,D3DXVECTOR3 mMax )
{
 this->mMax=mMax;
 this->mMin=mMin;
 mpIndexBuffer = NULL;
 mpVertexBuffer = NULL;
}
HRESULT  CSkyBox ::Construct ( LPDIRECT3DDEVICE9 pd3dDevice )
{    
	HRESULT hr;
	// create vertex buffer

	V_RETURN ( CreateVertexBuffer( pd3dDevice ,sizeof(SPosColTex),24,D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1,  D3DUSAGE_WRITEONLY ,D3DPOOL_MANAGED));
    pVdata=NULL;
	// lock the vb 

	V_RETURN(LockVB(0,(void**)& pVdata,0) );
	 
	// load the data in vb
    
	// Top Side


	*pVdata = SPosColTex(mMin.x,	mMax.y,		mMax.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 1 , 0 );  pVdata++;
	*pVdata = SPosColTex(mMin.x,	mMax.y,		mMin.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 0 , 0 );  pVdata++;
	*pVdata = SPosColTex(mMax.x,	mMax.y,		mMin.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 0 , 1 );  pVdata++;
	*pVdata = SPosColTex(mMax.x,	mMax.y,		mMax.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 1 , 1 );  pVdata++;




	// Bottom Side

	*pVdata = SPosColTex(mMax.x,	mMin.y,		mMin.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 0 , 0 );  pVdata++;
	*pVdata = SPosColTex(mMin.x,	mMin.y,		mMin.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 0, 1);  pVdata++;
	*pVdata = SPosColTex(mMin.x,	mMin.y,		mMax.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 1 , 1 );  pVdata++;
	*pVdata = SPosColTex(mMax.x,	mMin.y,		mMax.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 1 , 0 );  pVdata++;


	
	
	// Front Buffer
	*pVdata = SPosColTex(mMax.x,	mMax.y,		mMax.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 0 , 0 );  pVdata++;
	*pVdata = SPosColTex(mMax.x,	mMin.y,		mMax.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 0 , 1 );  pVdata++;
	*pVdata = SPosColTex(mMin.x,	mMin.y,		mMax.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 1 , 1 );  pVdata++;
	*pVdata = SPosColTex(mMin.x,	mMax.y,		mMax.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 1 , 0 );  pVdata++;



	// Back  side

	*pVdata = SPosColTex(mMin.x,	mMax.y,		mMin.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 0 , 0 );  pVdata++;
	*pVdata = SPosColTex(mMin.x,	mMin.y,		mMin.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 0 , 1 );  pVdata++;
	*pVdata = SPosColTex(mMax.x,	mMin.y,		mMin.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 1 , 1 );  pVdata++;
	*pVdata = SPosColTex(mMax.x,	mMax.y,		mMin.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 1 , 0 );  pVdata++;



		// Left Side

	*pVdata = SPosColTex(mMin.x,	mMax.y,		mMax.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 0 , 0 );  pVdata++;
	*pVdata = SPosColTex(mMin.x,	mMin.y,		mMax.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 0 , 1 );  pVdata++;
	*pVdata = SPosColTex(mMin.x,	mMin.y,		mMin.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 1 , 1 );  pVdata++;
	*pVdata = SPosColTex(mMin.x,	mMax.y,		mMin.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 1 , 0 );  pVdata++;



	//Right Side

	*pVdata = SPosColTex(mMax.x,	mMax.y,		mMin.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 0 , 0 );  pVdata++;
	*pVdata = SPosColTex(mMax.x,	mMin.y,		mMin.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 0 , 1 );  pVdata++;
	*pVdata = SPosColTex(mMax.x,	mMin.y,		mMax.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 1 , 1 );  pVdata++;
	*pVdata = SPosColTex(mMax.x,	mMax.y,		mMax.z, D3DCOLOR_ARGB( 255, 255, 255, 255), 1 , 0 );  pVdata++;

	


	// unlock the vb
	
	V_RETURN(UnLockVB());
	
	// load the texture from the file
	V_RETURN(D3DXCreateTextureFromFile(pd3dDevice,L"Data/stars0.jpg",&cube[0]));
	V_RETURN(D3DXCreateTextureFromFile(pd3dDevice,L"Data/stars1.jpg",&cube[1]));	
	V_RETURN(D3DXCreateTextureFromFile(pd3dDevice,L"Data/stars2.jpg",&cube[2]));	
	V_RETURN(D3DXCreateTextureFromFile(pd3dDevice,L"Data/stars3.jpg",&cube[3]));
	V_RETURN(D3DXCreateTextureFromFile(pd3dDevice,L"Data/stars4.jpg",&cube[4]));
	V_RETURN(D3DXCreateTextureFromFile(pd3dDevice,L"Data/stars5.jpg",&cube[5]));
	
	
	

return S_OK;



}
HRESULT CSkyBox:: Render ( LPDIRECT3DDEVICE9 pd3dDevice )
{ 
	HRESULT hr;
    int i;

	
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState ( 0,  D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState ( 0,  D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
	
	pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSU ,D3DTADDRESS_CLAMP);
	pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSV ,D3DTADDRESS_CLAMP);

	int sv = 0;
	for ( int i = 0; i < 6 ; i++)
	{
		pd3dDevice->SetTexture (0, cube[i]);

		V_RETURN(pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, sv, 2));
	
		sv=sv+4;

	}
	
	return hr;

	


}
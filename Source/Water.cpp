#include "dxut.h"
#include "Water.h"


CWater::~CWater(void)
{
	if ( mUnderWaterTex )
		mUnderWaterTex->Release();
	if ( mWaterTex )
		mWaterTex->Release();

}

CWater::CWater(  D3DXVECTOR3 minIn, D3DXVECTOR3 maxIn, float AlphaVal, string texName, float flowSpeed, string sUnderWaterTexName)
{
	mMin = minIn;
	mMax = maxIn;
	mAlphaval = AlphaVal;
	mWaterTexName = texName;

	mWaterTex = NULL;


	mpIndexBuffer = NULL;
	mpVertexBuffer = NULL;

	mFlowSpeed = flowSpeed;

	D3DXMatrixIdentity ( &mWaterTexMatrix );
	msUnderWaterTexName  = sUnderWaterTexName;
	mUnderWaterTex = NULL;
	bFrustumCulling = false;
	
}


HRESULT CWater::Construct ( LPDIRECT3DDEVICE9 pd3dDevice )
{
	HRESULT hr;

	// Create Vertex Buffer
	V_RETURN ( CreateVertexBuffer ( pd3dDevice, sizeof ( SPosColTex ), 4, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1,
			   D3DUSAGE_WRITEONLY, D3DPOOL_MANAGED ) );

	//	lock
	SPosColTex *pvData = NULL;
	V_RETURN ( LockVB(0, (void**)&pvData, 0 ) );

	int alphavalac = (int)( mAlphaval * 255 + 0.5f );
	int r = 0, g = 0, b = 200;
	// fill data
	*pvData = SPosColTex ( mMin.x, mMin.y, mMin.z, D3DCOLOR_ARGB ( alphavalac, r, g, b ), 0, 0 );	pvData++;
	*pvData = SPosColTex ( mMin.x, mMin.y, mMax.z, D3DCOLOR_ARGB ( alphavalac, r, g, b ), 0, 1 );	pvData++;
	*pvData = SPosColTex ( mMax.x, mMin.y, mMax.z, D3DCOLOR_ARGB ( alphavalac, r, g, b ), 1, 1 );	pvData++;
	*pvData = SPosColTex ( mMax.x, mMin.y, mMin.z, D3DCOLOR_ARGB ( alphavalac, r, g, b ), 1, 0 );	




	// unlock
	V_RETURN ( UnLockVB() );

	// Load the Texture

	TCHAR temp[200];
	mbstowcs ( temp, mWaterTexName.data(), 200 );
	V_RETURN ( D3DXCreateTextureFromFile ( pd3dDevice, temp, &mWaterTex ) );

	// Load Under Water texture
	
	mbstowcs ( temp, msUnderWaterTexName.data(), 200 );
	V_RETURN ( D3DXCreateTextureFromFile ( pd3dDevice, temp, &mUnderWaterTex ) );

	// Calculate the Plane
	mWaterPlane.a = 0.0f;
	mWaterPlane.b = 1.0f;
	mWaterPlane.c = 0.0f;
	// ax + by + cz = d => -(ax + bt + cz );

	mWaterPlane.d = 0  - ( mWaterPlane.a * mMin.x + mWaterPlane.b * mMin.y + mWaterPlane.c * mMin.z );

	return hr;
}
HRESULT CWater::Render ( LPDIRECT3DDEVICE9 pd3dDevice )
{
	HRESULT hr;
	// Set up Trilinear Filtering
	// For tex 0
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );


	// Set up Alpha Blending
	pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
	pd3dDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	pd3dDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	pd3dDevice->SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );


	// Set up stage 0

	//Get the Color Value from texture
	pd3dDevice->SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );

	// Get Alpha Value from Diffuse
	pd3dDevice->SetTextureStageState ( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
	pd3dDevice->SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );


	// Setup the	Flags for transformation
	pd3dDevice->SetTextureStageState ( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 /* 2D Texture */ );

	// set up the matrix
	pd3dDevice->SetTransform ( D3DTS_TEXTURE0, &mWaterTexMatrix );

	//Set the Texture
	V_RETURN ( pd3dDevice->SetTexture ( 0, mWaterTex ));
	
	// Set FVF, vb
	V_RETURN ( SetFVF ( pd3dDevice ) );
	V_RETURN ( SetStreamSource ( pd3dDevice, 0, 0 ));

	//	Draw
	V_RETURN ( pd3dDevice->DrawPrimitive ( D3DPT_TRIANGLEFAN, 0, 2 ) );

	
	// Disable Textrure Transformation
	pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
	
	// Draw UnderWaterPoly
	if ( bFrustumCulling )
		V_RETURN ( DrawUnderWaterPoly ( pd3dDevice ));

	//	Set Tex0 to NULL
	V_RETURN ( pd3dDevice->SetTexture ( 0, NULL ));


	// disabled alpha blending
	pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, FALSE );

	// disable the texture tranformation
	pd3dDevice->SetTextureStageState ( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );

	return hr;

}

void CWater::UpdateWaterMatrix ( float elapsedTime )
{
	float dist = mFlowSpeed * elapsedTime ;
	mWaterTexMatrix._31 += dist;

	if ( mWaterTexMatrix._31 > 1.0f )
		mWaterTexMatrix._31 -= 1.0f;
}
void CWater::GetUnderWaterPolyInfoArray ( D3DXVECTOR3 **vecIn, float **uvs )
{
	*vecIn	= mNearPlane;
	*uvs	= mfUVNearPlane;
}

HRESULT CWater::DrawUnderWaterPoly(  LPDIRECT3DDEVICE9 pd3dDevice )
{
	HRESULT hr;
	// Update the SPosColRender Array

	int alpha = 120, r, g, b;
	r = g = b = 255;
	for (int i = 0 ; i < 4; i++)
	{
		mNearPlaneRender[i] = SPosColTex ( mNearPlane[i].x, 
										   mNearPlane[i].y, 
										   mNearPlane[i].z, 
										   D3DCOLOR_ARGB ( alpha, r, g, b ), 
										   mfUVNearPlane [ 2*i + 0 ],
										   mfUVNearPlane [ 2*i + 1 ]
										 );
	}
	// Set the Under Water Texture
	 V_RETURN ( pd3dDevice->SetTexture ( 0, mUnderWaterTex ) );

	// Draw
	V_RETURN ( pd3dDevice->DrawPrimitiveUP ( D3DPT_TRIANGLEFAN, 2, mNearPlaneRender, sizeof ( SPosColTex) ));
	return hr;
}

D3DXPLANE CWater::GetWaterPlane()
{
	return mWaterPlane;
}
void CWater::setUnderWaterPoly ( bool truefalse )
{
	bFrustumCulling = truefalse;
}

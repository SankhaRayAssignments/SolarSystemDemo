#include "dxut.h"
#include "CSphereObject.h"

CSphereObject::CSphereObject(CSphere *Mesh, float scale, string texName )
{
	mpMesh = Mesh;
	mScale = scale;
	mTexName = texName;
	D3DXMatrixIdentity ( &mWorldWithOutScale );
}

CSphereObject::~CSphereObject(void)
{
	SAFE_RELEASE ( mTexture );

}

HRESULT CSphereObject::Construct(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr;
	// Create The Textures

	TCHAR temp[200];
	mbstowcs ( temp, mTexName.data(), 200 );
	V_RETURN ( D3DXCreateTextureFromFile ( pd3dDevice, temp, &mTexture ) );

	return hr;
}
void CSphereObject::SetAtmosphere ( bool val )
{
	mAtmosphereVisible = val ;
}

HRESULT CSphereObject::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr;

	if ( mAtmosphereVisible )
	{
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
		//	pd3dDevice->SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		//	pd3dDevice->SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );

		// Get Alpha Value from Diffuse
		pd3dDevice->SetTextureStageState ( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
		pd3dDevice->SetTextureStageState ( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );


		// Setup the	Flags for transformation
		pd3dDevice->SetTextureStageState ( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 /* 2D Texture */ );
	}

	V_RETURN ( pd3dDevice->SetTexture ( 0, mTexture ) );
	V_RETURN ( mpMesh->SetFVFVBIBOnDevice(pd3dDevice) );

	D3DXMATRIXA16 WorldMat;
	D3DXMatrixIdentity ( &WorldMat );




	D3DXMATRIXA16 ScaleMat;
	D3DXMatrixScaling ( &ScaleMat, mScale, mScale, mScale );
	mWorldWithOutScale = mWorldTransform;

	WorldMat = ScaleMat * mWorldTransform;

	pd3dDevice->SetTransform( D3DTS_WORLD, &WorldMat);

	pd3dDevice->SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );
	V_RETURN ( mpMesh->DrawIndexedTriStrip ( pd3dDevice, 0, 0, 0 ) );
	if ( mAtmosphereVisible )
	{
		pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, FALSE);
	}
	//pd3dDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, FALSE);
	//pd3dDevice->SetSamplerState ( 0, D3DSAMP_MINFILTER, D3DTEXF_NONE );
	//pd3dDevice->SetSamplerState ( 0, D3DSAMP_MAGFILTER, D3DTEXF_NONE );
	//pd3dDevice->SetSamplerState ( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
	//pd3dDevice->SetTextureStageState ( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
	//pd3dDevice->SetTextureStageState ( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE /* 2D Texture */ );
		V_RETURN ( pd3dDevice->SetTexture ( 0, NULL ) );


	pd3dDevice->SetRenderState ( D3DRS_CULLMODE, D3DCULL_CW );

	return hr; 
}

void CSphereObject::GetWorldMatrixWithOutScale (D3DXMATRIXA16& matIn) 
{
	matIn = mWorldWithOutScale;
}
void CSphereObject::GetWorldMatrix (D3DXMATRIXA16& matIn) 
{
	D3DXMATRIXA16 ScaleMat;
	D3DXMatrixScaling ( &ScaleMat, mScale, mScale, mScale );
	matIn = mWorldTransform;
}
float CSphereObject::GetScale ()
{
	return mScale;
}



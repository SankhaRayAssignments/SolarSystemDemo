#pragma once
#include "CSphere.h"
#include "CObject.h"
#include "COrbit.h"

class CSphereObject : public CObject
{
protected:
	float mScale;
	string mTexName;
	LPDIRECT3DTEXTURE9 mTexture;

	D3DXMATRIXA16 mWorldWithOutScale;
	bool mAtmosphereVisible;
	float mfTilt;
	



public:
	CSphereObject(CSphere *Mesh = NULL, float scale = 1.0f,  string texName = NULL);

	virtual ~CSphereObject(void);

	virtual HRESULT Construct(LPDIRECT3DDEVICE9 pd3dDevice);
	virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice);
	void GetWorldMatrix (D3DXMATRIXA16& matIn) ;
	void GetWorldMatrixWithOutScale (D3DXMATRIXA16& matIn) ;
	float GetScale ();
	void SetAtmosphere ( bool val );
	void setTilt ( float tilt ) { mfTilt = tilt; }
	

};

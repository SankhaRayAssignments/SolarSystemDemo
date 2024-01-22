#pragma once
#include "CMesh.h"
#include <string>
using namespace std;

#define TOTLAT 180.0f
#define TOTLONG 360.0f

class CSphere : public CMesh
{
protected:

	D3DXMATRIXA16 mWorldMatrix;
	string mTexName;
	LPDIRECT3DTEXTURE9 mTexture;
	int mRadius;
	D3DXVECTOR3 mBangCoord;
	float mLat, mLong;

public:
	CSphere(  string texName, float lat = 0.0f,  float Long = 0.0f);
	virtual ~CSphere(void);
	virtual HRESULT Construct(LPDIRECT3DDEVICE9 pd3dDevice);
	virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice);
	D3DXVECTOR3 GetBangCoord ( );
};

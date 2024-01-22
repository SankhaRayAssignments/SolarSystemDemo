#pragma once
#include "cmesh.h"
#include <string>
using namespace std;

class CWater : 	public CMesh
{
protected:

	D3DXVECTOR3 mMin; 
	D3DXVECTOR3 mMax;
	string mWaterTexName;
	float mAlphaval;
	LPDIRECT3DTEXTURE9 mWaterTex;

	D3DXMATRIXA16 mWaterTexMatrix;
	float mFlowSpeed;

	D3DXVECTOR3	mNearPlane [ 4 ]; 
	float mfUVNearPlane [ 8 ];

	string msUnderWaterTexName;
	LPDIRECT3DTEXTURE9 mUnderWaterTex;

	SPosColTex mNearPlaneRender [8];

	D3DXPLANE mWaterPlane ;
	bool bFrustumCulling;

public:
	CWater(void)  {}
	CWater(  D3DXVECTOR3 minIn, D3DXVECTOR3 maxIn, float AlphaVal, string texName, float flowSpeed , string sUnderWaterTexName);
	virtual ~CWater(void);

	virtual HRESULT Construct ( LPDIRECT3DDEVICE9 pd3dDevice );
	virtual HRESULT Render ( LPDIRECT3DDEVICE9 pd3dDevice );
	void UpdateWaterMatrix ( float elapsedTime );
	void GetUnderWaterPolyInfoArray ( D3DXVECTOR3 **vecIn, float **uvs );
	HRESULT DrawUnderWaterPoly(  LPDIRECT3DDEVICE9 pd3dDevice );
	D3DXPLANE GetWaterPlane();
	void setUnderWaterPoly ( bool truefalse );

};

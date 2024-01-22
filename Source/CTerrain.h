
#pragma once
#include "CMesh.h"
#include "CTGA.h"
/*
#include <string>
using namespace std;
*/

class CTerrain : public CMesh
{

//vars
public:

//funcs
public:
	CTerrain();
	CTerrain(D3DXVECTOR3 min, D3DXVECTOR3 max, int rows, int cols, string fname, float DetailTexScale, string BaseTextName, string DetailTexName);
	virtual ~CTerrain();
	virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice);
	virtual HRESULT Construct(LPDIRECT3DDEVICE9 pd3dDevice);

	float GetHeightValInterpolated(float row, float col);
	float GetHeightVal(int row, int col);
	void GetRowColGivenWorldPos(D3DXVECTOR3& vecIn, float& row, float& col);

	void SetDeatilTextureOnOff ( bool DetailTextureOnOff );
//	bool isBasePointInsideTerrain ( const D3DXVECTOR3 pt1,const D3DXVECTOR3 pt2,const D3DXVECTOR3 pt3,const D3DXVECTOR3 pt4 );

//vars
protected:
	D3DXVECTOR3 mMin, mMax;
	int mRows, mCols;
	string mHeightMapName;
	CTGA* mpHeightMap;
	UCHAR* mpHmapData;
	D3DXVECTOR3 mLightDir; //game will set this later
	float mDelX, mDelZ;

	float mDetailTexScale;
	string mBaseTexName, mDetailTexName;

	LPDIRECT3DTEXTURE9 mBaseTex, mDetailTex;


	bool bDetailTextureOnOff;

//funcs
protected:
	
	D3DXVECTOR3 GetNormal(int row, int col);

};
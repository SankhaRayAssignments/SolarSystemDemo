#pragma once
#include "CMesh.h"

class CCuboid : public CMesh
{

//funcs
public:
	CCuboid();
	CCuboid(D3DXVECTOR3 min, D3DXVECTOR3 max);
	virtual ~CCuboid();
	virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice);
	virtual HRESULT Construct(LPDIRECT3DDEVICE9 pd3dDevice);
	virtual void GetMin(D3DXVECTOR3& minIn) { minIn = mMin; }
	virtual void GetMax(D3DXVECTOR3& maxIn) { maxIn = mMax; }
//	void SetMinMax(D3DXVECTOR3& maxIn, D3DXVECTOR3& maxIn) ;
	
//vars
protected:
	D3DXVECTOR3 mMin, mMax;
	float mWidth, mHeight, mDepth;

};
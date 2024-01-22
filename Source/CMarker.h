#pragma once
#include "cmesh.h"
#include <string>
using namespace std;

class CMarker : public CMesh
{
protected:
	D3DXVECTOR3 mMin;
	D3DXVECTOR3 mMax;
	string mTexName;
	LPDIRECT3DTEXTURE9 mTexture;
	D3DXMATRIXA16 mWorldMatrix;
public:
	CMarker(D3DXVECTOR3 minIn, D3DXVECTOR3 maxIn, string texName);
	virtual ~CMarker(void);
	//override in derived classes to render strips, lists etc.
	virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice);

	//override in derived classes to construct according to desired geometry
	virtual HRESULT Construct(LPDIRECT3DDEVICE9 pd3dDevice);

	void UpdateWorldMatrix ( D3DXMATRIXA16& matIn );

};

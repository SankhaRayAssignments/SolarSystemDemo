#pragma once
#include "cmesh.h"

class CSkyBox : public CMesh
{
public:
	CSkyBox(){};
	 virtual  ~ CSkyBox(void);
	CSkyBox(D3DXVECTOR3 mMin ,D3DXVECTOR3 mMax );
    virtual HRESULT Construct ( LPDIRECT3DDEVICE9 pd3dDevice );
	virtual HRESULT Render ( LPDIRECT3DDEVICE9 pd3dDevice );
  protected:
	  D3DXVECTOR3 mMax;
	  D3DXVECTOR3 mMin;
	  LPDIRECT3DTEXTURE9 cube[6];
	  SPosColTex * pVdata;


};

#pragma once
#include "CSphereObject.h"

// Assume 1 day = 1 secs
#define TIMEFACTOR 10.0f
#define DEFAULTSCALE 1.0f

class CPlanet : public CSphereObject
{
protected:
	float mRevolutionTime;
	float mRotationTime;

	float mCurrRotAngle;
	float mCurrRevAngle;
	float mDistance;

	D3DXMATRIXA16 mParentWorldPos;
	COrbit *mOrbit;



//	D3DXVECTOR3 mOrbitCoord;

public:
	CPlanet(CSphere *Mesh = NULL, float distance = 0.0f, float rev_time = TIMEFACTOR, float rot_time = TIMEFACTOR, float scale = DEFAULTSCALE ,  string texName = NULL);
	virtual ~CPlanet(void);
	void UpdatePosition ( float fElapsedTime );

	void SetParentWorldMat ( D3DXMATRIXA16& matIn );
	D3DXVECTOR3 GetCenterCoord ();

	HRESULT Construct(LPDIRECT3DDEVICE9 pd3dDevice);
	HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice);


};

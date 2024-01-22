#pragma once
#include "CCamera.h"

class CTPCamera : public CCamera
{

//vars
public:

//funcs
public:
	CTPCamera() { /*dont use this*/ }
	CTPCamera(float speed, float lagFrac);
	~CTPCamera();
	virtual void UpdateViewMatrixParams(D3DXVECTOR3& eye, D3DXVECTOR3& lookAt, D3DXVECTOR3& upVec, bool bResetVals = false);

//vars
protected:
	float mLagFraction; //dist moved = (targetPos-CurrenPos).length*mLagFraction
	D3DXVECTOR3 mTargetPos;
	bool mFirstUpdateDone;

//funcs
protected:

};
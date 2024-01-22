#pragma once
#include <d3d9.h>
#include "CObject.h"
#include "CInput.h"

class CCamera : public CObject
{
	//vars
public:

	//funcs
public:
	CCamera() {}
	CCamera(float speed);
	virtual ~CCamera();

	void UpdateParamsOnInput(CInput* inp, float fElapsedTime);
	virtual void UpdateViewMatrixParams(D3DXVECTOR3& eye, D3DXVECTOR3& lookAt, D3DXVECTOR3& upVec, bool bResetVals = false);
	void UpdateViewMatrix();
	void UpdateProjMatrix(float fov, float aspect, float zn, float zf, int w, int h);

	D3DXMATRIXA16* GetViewMatrix() { return &mViewMat; }
	D3DXMATRIXA16* GetProjMatrix() { return &mProjMat; }



	//vars
protected:
	D3DXMATRIXA16 mViewMat;
	D3DXMATRIXA16 mProjMat;
	D3DXVECTOR3	  mEyePos, mLookAtPoint, mUpVec;
	D3DXVECTOR3	  mResetEyePos, mResetLookAtPoint, mResetUpVec;

	D3DXVECTOR3   mLookVec,mRightVec;

	float mFOV, mAspect, mZNear, mZFar;






	//funcs
protected:
	void ComputeLookVec();
	void ComputeRightVec();

};
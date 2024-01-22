#include "DXUT.h"
#include "ThirdPersonCam.h"

CTPCamera::CTPCamera(float speed, float lagFrac):CCamera(speed)
{
	mLagFraction = lagFrac;
	mFirstUpdateDone = false;
}
	
CTPCamera::~CTPCamera()
{
}
	
void CTPCamera::UpdateViewMatrixParams(D3DXVECTOR3& eye, D3DXVECTOR3& lookAt, 
									   D3DXVECTOR3& upVec, bool bResetVals /*= false*/)
{

	if(!mFirstUpdateDone)
	{
		mEyePos = mTargetPos = eye;
		mFirstUpdateDone = true;
	}
	else
	{
		mTargetPos = eye;
		D3DXVECTOR3 tVec = mTargetPos - mEyePos;
		float len = D3DXVec3Length(&tVec);

		if(len < 0.101f )
		{
			mEyePos = mTargetPos;
		}
		else
		{
			D3DXVec3Normalize( &tVec, &tVec );
			mEyePos = mEyePos + ( len*mLagFraction )*tVec;
		}
	}

	mLookAtPoint = lookAt;
	mUpVec = upVec;

	ComputeLookVec(); 
	ComputeRightVec();
	UpdateViewMatrix();

}

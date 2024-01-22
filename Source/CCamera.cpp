#include "DXUT.h"
#include "CCamera.h"


CCamera::CCamera(float speed)
{
	mSpeed = speed;
}


CCamera::~CCamera()
{
}

void CCamera::ComputeLookVec()
{
	//assumes only mLookAt and mEyePos have the desired values already
	mLookVec = mLookAtPoint-mEyePos;
	D3DXVec3Normalize(&mLookVec,&mLookVec);

}

void CCamera::ComputeRightVec()
{
	//assumes mLookVec and mUpVec have the desired values already and are unit length

	D3DXVec3Cross(&mRightVec,&mLookVec,&mUpVec);
	D3DXVec3Normalize(&mRightVec,&mRightVec);
}


void CCamera::UpdateParamsOnInput(CInput* inp, float fElapsedTime)
{
	//basic cam gives us the flycam behaviour

	float dist = fElapsedTime*mSpeed;

	//TCHAR debugMsg[100];

	//first the mouse so that look, up, right vecs are updated
	//
	bool mouseMoved = false;
	if(inp->MouseMoved())
	{
		mouseMoved = true;

		long mousex, mousey, mousez;
		mousex = mousey = mousez = 0;
		D3DXVECTOR3 temp = mLookAtPoint-mEyePos;
		float len = D3DXVec3Length(&temp);

		inp->GetDeltaMousePos(mousex, mousey, mousez);


		D3DXMATRIXA16 rotMat;

		if(mousex != 0) //yaw
		{ 
			//upvec is the same, recompute the other two
			D3DXVec3Normalize(&mUpVec, &mUpVec);
			D3DXMatrixRotationAxis(&rotMat, &mUpVec, D3DXToRadian(-mousex/10.0f) );

			//StringCchPrintf(debugMsg, 100, L"%ld \n",mousex);
			//OutputDebugString(debugMsg);

			//assuming the function  normalizes after rotation as well... have to check it
			D3DXVec3TransformNormal(&mLookVec,&mLookVec,&rotMat); 
			D3DXVec3TransformNormal(&mRightVec,&mRightVec,&rotMat); 

		}

		if(mousey != 0) //pitch
		{
			
			//rightvec is the same, recompute the other two
			D3DXVec3Normalize(&mRightVec, &mRightVec);
			D3DXMatrixRotationAxis(&rotMat, &mRightVec, D3DXToRadian(-mousey/10.0f) );
			//assuming the function  normalizes after rotation as well... have to check it
			D3DXVec3TransformNormal(&mLookVec,&mLookVec,&rotMat); 
			D3DXVec3TransformNormal(&mUpVec,&mUpVec,&rotMat); 
			

		}

		if(mousez != 0) //roll
		{
			//lookvec is the same, recompute the other two
			D3DXVec3Normalize(&mLookVec, &mLookVec);
			D3DXMatrixRotationAxis(&rotMat, &mLookVec, D3DXToRadian(-mousez/100.0f) );
			//assuming the function  normalizes after rotation as well... have to check it
			D3DXVec3TransformNormal(&mRightVec,&mRightVec,&rotMat); 
			D3DXVec3TransformNormal(&mUpVec,&mUpVec,&rotMat); 

		}

		//update lookAt
		mLookAtPoint = mEyePos + len*mLookVec;
		
	}


	//second the keyboard

	if(inp->CheckKeyPressed(DIK_W))
	{
		if(!mouseMoved) ComputeLookVec(); 
		mLookAtPoint = mLookAtPoint + dist*mLookVec;
		mEyePos = mEyePos + dist*mLookVec;
	}
	else if(inp->CheckKeyPressed(DIK_S))
	{
		if(!mouseMoved) ComputeLookVec(); 
		mLookAtPoint = mLookAtPoint - dist*mLookVec;
		mEyePos = mEyePos - dist*mLookVec;

	}

	if(inp->CheckKeyPressed(DIK_D))
	{
		if(!mouseMoved) ComputeRightVec(); 
		mLookAtPoint = mLookAtPoint + dist*mRightVec;
		mEyePos = mEyePos + dist*mRightVec;
	}
	else if(inp->CheckKeyPressed(DIK_A))
	{
		if(!mouseMoved) ComputeRightVec(); 
		mLookAtPoint = mLookAtPoint - dist*mRightVec;
		mEyePos = mEyePos - dist*mRightVec;

	}


	if(inp->CheckKeyJustUp(DIK_F4))
	{

		UpdateViewMatrixParams(mResetEyePos, mResetLookAtPoint, mResetUpVec);
	}


	UpdateViewMatrix();

	
}

void CCamera::UpdateViewMatrixParams(D3DXVECTOR3& eye, D3DXVECTOR3& lookAt, D3DXVECTOR3& upVec, bool bResetVals /*= false*/)
{
	mEyePos = eye;
	mLookAtPoint = lookAt;
	mUpVec = upVec;

	if(bResetVals)
	{
		mResetEyePos = mEyePos;
		mResetLookAtPoint = mLookAtPoint;
		mResetUpVec = mUpVec;
	}

	ComputeLookVec(); 
	ComputeRightVec();
	UpdateViewMatrix();
}

void CCamera::UpdateViewMatrix()
{
	D3DXMatrixLookAtRH(&mViewMat, &mEyePos, &mLookAtPoint, &mUpVec);
}

void CCamera::UpdateProjMatrix(float fov, float aspect, float zn, float zf, int w, int h)
{
	mFOV = fov;
	mAspect = aspect;
	mZNear = zn;
	mZFar = zf;

	D3DXMatrixPerspectiveFovRH(&mProjMat, mFOV, mAspect, mZNear, mZFar);
}
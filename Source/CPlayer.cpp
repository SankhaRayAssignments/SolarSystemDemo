#include "DXUT.h"
#include "CPlayer.h"

CPlayer::CPlayer() : CObject()
{
	//dont use the default constructor
}


CPlayer::CPlayer(CMesh* mesh, float speed, float gravSpeed, float friction, float x, float y, float z,
				 float rx, float ry, float rz) : CObject(x,y,z,rx,ry,rz)
{

	mCurrentPos = D3DXVECTOR3(x,y,z);

	mRotAngle = rx;
	mRotMat = mWorldTransform;
	mRotMat._41 = mRotMat._42 = mRotMat._43 = 0.0f;

	mLookForward = D3DXVECTOR3(0,0,-1);
	mRightVec = D3DXVECTOR3(1,0,0);
	mUpVec = D3DXVECTOR3(0,1,0);

	D3DXVec3TransformNormal(&mLookForward,&mLookForward,&mRotMat); 
	D3DXVec3TransformNormal(&mRightVec,&mRightVec,&mRotMat); 

	mCurrentDirection = mLookForward;

	mSpeed = speed;
	mCurrSpeed = 0;
	mGravitySpeed = gravSpeed;
	mFriction = friction;
	mpMesh = mesh;
	mbIsFalling = true; //this is important because we will set the player just above the terrain
	mbIsMoving = false;
	mbKeyPressed = false;
	mbMouseMoved = false;
	
	mFPCamPitch = 0;
	UpdateFPCamParams();

}

CPlayer::~CPlayer()
{
}

void CPlayer::UpdateParamsOnInput(CInput* inp, float fElapsedTime)
{
	//mover the player - mouse move horizontal to change yaw, not pitch/roll
	//keyboard moves the player in direction where he is looking

	float dist = fElapsedTime * mCurrSpeed;
	float downDist = fElapsedTime * mGravitySpeed;
	

	//TCHAR debugMsg[100];

	//first the mouse so that look, right vecs are updated
	//

	
	D3DXVECTOR3 lookForward = D3DXVECTOR3(0,0,-1);
	D3DXVECTOR3 rightVec = D3DXVECTOR3(1,0,0);

	long mousex, mousey, mousez;


	mbMouseMoved = false;
	if(inp->MouseMoved())
	{
		mbMouseMoved = true;

		mousex = mousey = mousez = 0;

		inp->GetDeltaMousePos(mousex, mousey, mousez);

		if(mousex != 0) //yaw
		{ 
			//mRotAngle accumulated
			mRotAngle += ( -mousex / 10.0f);
			if(mRotAngle > 360.0f) mRotAngle = mRotAngle - 360.0f;
			if(mRotAngle < 0.0f) mRotAngle = mRotAngle + 360.0f;


			//mRotMat, mLookForward, mRightVec calculated if mousemoved horizontally
			D3DXMatrixRotationAxis(&mRotMat, &mUpVec, D3DXToRadian(mRotAngle) );
			D3DXVec3TransformNormal(&mLookForward,&lookForward,&mRotMat); 
			D3DXVec3TransformNormal(&mRightVec,&rightVec,&mRotMat); 
		}

		//pitch, for the FirstPersonCam  coordinates attached to the player FPCam
		if(mousey != 0) 
		{
			mFPCamPitch += (-mousey/10.0f);
			if(mFPCamPitch > 80.0f) mFPCamPitch = 80.0f;
			if(mFPCamPitch < -80.0f) mFPCamPitch = -80.0f;

			//DXUTOutputDebugString( L"mFPCamPitch: %f\n", mFPCamPitch );
		}
		
	}

	//second the keyboard
	mbKeyPressed = false;
	if(inp->CheckKeyPressed(DIK_W))
	{
		mCurrentPos = mCurrentPos + dist*mLookForward;
		mbKeyPressed = true;
		mCurrentDirection = mLookForward;
	}
	else if(inp->CheckKeyPressed(DIK_S))
	{
		mCurrentPos = mCurrentPos - dist*mLookForward;
		mbKeyPressed = true;
		mCurrentDirection = -mLookForward;
	}

	if(inp->CheckKeyPressed(DIK_D))
	{
		//doing it insise since user can press W/A, W/D simultaneously
		mCurrentPos = mCurrentPos + dist*mRightVec;
		mbKeyPressed = true;
		mCurrentDirection = mRightVec;
	}
	else if(inp->CheckKeyPressed(DIK_A))
	{
		mCurrentPos = mCurrentPos - dist*mRightVec;
		mbKeyPressed = true;
		mCurrentDirection = -mRightVec;
	}


	//third, displacemnt due to gravity
	mCurrentPos = mCurrentPos - downDist*D3DXVECTOR3(0,1,0);

	//fourth, if key was not pressed, move with "remaining velocity"
	if(mbKeyPressed)
	{
		mCurrSpeed = mSpeed;
	}
	else
	{
		if(mCurrSpeed > 0.01f)
		{
			mbIsMoving = true;
			mCurrentPos = mCurrentPos + dist * mCurrentDirection;
			mCurrSpeed = mCurrSpeed - 1000* fElapsedTime * mFriction* mCurrSpeed;
		}
		else
		{
				mCurrSpeed = 0.0f;
				mbIsMoving = false;
		}
	}

	if(mbMouseMoved || mbKeyPressed || mbIsFalling || mbIsMoving)
	{
		UpdateWorldTransform();

		UpdateFPCamParams();

		ComputeTPCamEyePos();
	}
}

void CPlayer::UpdateFPCamParams()
{
	

	D3DXMATRIXA16 tempMat;
	D3DXMatrixRotationAxis(&tempMat, &mRightVec, D3DXToRadian(mFPCamPitch) );
	D3DXVec3TransformNormal(&mFPCamUpVec,&mUpVec,&tempMat); 
	D3DXVec3TransformNormal(&mFPCamLookVec, &mLookForward, &tempMat);

	ComputeFPCamEyePos();
	mFPCamLookAtPoint = mFPCamEyePos + mFPCamLookVec;

}

void CPlayer::UpdateWorldTransform()
{
	//D3DXMATRIXA16 mTransMat;
	//D3DXMatrixTranslation(&mTransMat, mCurrentPos.x, mCurrentPos.y, mCurrentPos.z);

	//D3DXMatrixMultiply(&mWorldTransform, &mRotMat, &mTransMat);
	mWorldTransform = mRotMat;
	mWorldTransform._41 = mCurrentPos.x;
	mWorldTransform._42 = mCurrentPos.y;
	mWorldTransform._43 = mCurrentPos.z;
	
}


void CPlayer::SetTPCamYPositon (float y)
{
	if(mTPCamEyePos.y < y)
	{
		mTPCamEyePos.y = y;
	}
}


void CPlayer::SetTerrainYPositon (float y)
{
	if(mCurrentPos.y < y)
	{
		mCurrentPos.y = y;
		mWorldTransform._42 = mCurrentPos.y;
		mbIsFalling = false;
	}
	else
	{
		mbIsFalling = true;
	}
}
void CPlayer::SetTerrainXZPositon(float x, float z )
{
	if(mCurrentPos.x > x)
	{
		mCurrentPos.x = x;
	}
	if(mCurrentPos.z > z)
	{
		mCurrentPos.z = z;
	}
}

void CPlayer::ComputeFPCamEyePos()
{
	D3DXVECTOR3 minCuboidExt, maxCuboidExt;

	mpMesh->GetMin(minCuboidExt);
	mpMesh->GetMax(maxCuboidExt);

	mFPCamEyePos = D3DXVECTOR3((minCuboidExt.x + maxCuboidExt.x) / 2.0f, 
		maxCuboidExt.y, minCuboidExt.z);
	D3DXVECTOR4 temp2;

	D3DXVec3Transform(&temp2, &mFPCamEyePos, &mWorldTransform);
	mFPCamEyePos.x = temp2.x;
	mFPCamEyePos.y = temp2.y;
	mFPCamEyePos.z = temp2.z;

}

void CPlayer::ComputeTPCamEyePos()
{
	D3DXVECTOR3 minCuboidExt, maxCuboidExt;

	mpMesh->GetMin(minCuboidExt);
	mpMesh->GetMax(maxCuboidExt);

	//Third Person cam eye position will be set such that it will be 
	//at a height of a*height of cuboid and distance of b*height of cuboid
	//right now a = 1.5, b = 2.0 -- can play with these values

	float a = 1.5f;
	float b = 4.0f;
	float height = maxCuboidExt.y - minCuboidExt.y;

	mTPCamEyePos = D3DXVECTOR3((minCuboidExt.x + maxCuboidExt.x)/2.0f, 
		maxCuboidExt.y + (a-1.0)*height , maxCuboidExt.z + b*height);
	D3DXVECTOR4 temp2;

	D3DXVec3Transform(&temp2, &mTPCamEyePos, &mWorldTransform);
	mTPCamEyePos.x = temp2.x;
	mTPCamEyePos.y = temp2.y;
	mTPCamEyePos.z = temp2.z;

}


void CPlayer::Get4BasePoints(D3DXVECTOR3& p1, D3DXVECTOR3& p2, D3DXVECTOR3& p3, D3DXVECTOR3& p4)
{
	//return the positions of 4 bottom points of the cuboid (userful for terrain check)
	D3DXVECTOR3 minCuboidExt, maxCuboidExt;

	mpMesh->GetMin(minCuboidExt);
	mpMesh->GetMax(maxCuboidExt);

	p1 = D3DXVECTOR3(minCuboidExt.x, 0, minCuboidExt.z);
	p2 = D3DXVECTOR3(minCuboidExt.x, 0, maxCuboidExt.z);
	p3 = D3DXVECTOR3(maxCuboidExt.x, 0, maxCuboidExt.z);
	p4 = D3DXVECTOR3(maxCuboidExt.x, 0, minCuboidExt.z);

	D3DXVECTOR4 temp;

	D3DXVec3Transform(&temp, &p1, &mWorldTransform); 
	p1.x = temp.x; p1.y = temp.y; p1.z = temp.z;

	D3DXVec3Transform(&temp, &p2, &mWorldTransform);
	p2.x = temp.x; p2.y = temp.y; p2.z = temp.z;

	D3DXVec3Transform(&temp, &p3, &mWorldTransform);
	p3.x = temp.x; p3.y = temp.y; p3.z = temp.z;

	D3DXVec3Transform(&temp, &p4, &mWorldTransform);
	p4.x = temp.x; p4.y = temp.y; p4.z = temp.z;

}
void CPlayer::Set4BasePoints(D3DXVECTOR3& p1, D3DXVECTOR3& p2, D3DXVECTOR3& p3, D3DXVECTOR3& p4)
{
	D3DXVECTOR3 Min_, Max_;
	mpMesh->GetMin ( Min_ );
	mpMesh->GetMax ( Max_ );
	Min_.x = p1.x;
	Min_.z = p1.z;
//	mpMesh->SetMinMax ( Max_) ;
}

HRESULT CPlayer::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr;

	pd3dDevice->SetTransform( D3DTS_WORLD, &mWorldTransform );

	V_RETURN(mpMesh->SetFVFVBIBOnDevice(pd3dDevice));
	V_RETURN(mpMesh->Render(pd3dDevice));
	
	return S_OK;
}
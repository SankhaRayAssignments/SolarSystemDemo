#pragma once
#include "CObject.h"

class CPlayer : public CObject
{
//vars
public:

//funcs
public:
	CPlayer();
	CPlayer(CMesh* mesh, float speed, float gravSpeed, float friction, float x, float y, float z,
				 float rx, float ry, float rz);
	virtual ~CPlayer();
	void UpdateParamsOnInput(CInput* inp, float fElapsedTime);
	
	virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice);
	void Get4BasePoints(D3DXVECTOR3& p1, D3DXVECTOR3& p2, D3DXVECTOR3& p3, D3DXVECTOR3& p4);
	void Set4BasePoints(D3DXVECTOR3& p1, D3DXVECTOR3& p2, D3DXVECTOR3& p3, D3DXVECTOR3& p4);
	void SetTerrainYPositon (float y);
	void SetTerrainXZPositon(float x, float z );
	bool IsInMotion() { return (mbMouseMoved || mbKeyPressed || mbIsFalling || mbIsMoving); }

	//function for FPCam
	D3DXVECTOR3& GetFPCamEyePos() { return mFPCamEyePos; }
	D3DXVECTOR3& GetFPCamUpVec() { return mFPCamUpVec; }
	D3DXVECTOR3& GetFPCamLookAtPoint() { return mFPCamLookAtPoint; }

	//funcions for TPCam
	D3DXVECTOR3& GetTPCamEyePos() { return mTPCamEyePos; }
	//player's up vec is the Third person cam's upVec also
	D3DXVECTOR3& GetTPCamUpVec() { return mUpVec; }
	//the First person cam's eye pos will be the look at point for Third person Cam
	D3DXVECTOR3& GetTPCamLookAtPoint() { return mFPCamEyePos; } 
	//TPCam eyepos should not be underground
	void SetTPCamYPositon(float y);



//vars
protected:
	float mRotAngle;
	D3DXVECTOR3 mCurrentPos;
	D3DXMATRIXA16 mRotMat;
	D3DXVECTOR3 mLookForward;
	D3DXVECTOR3 mRightVec;
	D3DXVECTOR3 mUpVec;
	D3DXVECTOR3 mCurrentDirection;
	bool mbIsFalling;
	bool mbIsMoving;
	bool mbKeyPressed;
	bool mbMouseMoved;
	float mCurrSpeed;
	float mFriction;
	float mGravitySpeed;

	//parameters to attach a FirstPerson Cam
	float mFPCamPitch;
	D3DXVECTOR3 mFPCamEyePos;
	D3DXVECTOR3 mFPCamUpVec;
	D3DXVECTOR3 mFPCamLookVec;
	D3DXVECTOR3 mFPCamLookAtPoint;

	//parameters to attach a ThirdPerson cam
	D3DXVECTOR3 mTPCamEyePos;


//funcs
protected:
	void ComputeFPCamEyePos();
	void ComputeTPCamEyePos();
	void UpdateWorldTransform();
	void UpdateFPCamParams();

};
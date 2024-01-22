#pragma once
#include "cmesh.h"
#include "math.h"

#define POINT_ON_PLANE 0
#define POINT_ON_POSITIVE_PLANE 1
#define POINT_ON_NEGETIVE_PLANE -1

class CFrustum : public CMesh
{
protected:
	float mZNear, mZFar, mFOV, mAspectRatio ;

	D3DXVECTOR3 mFrustumVertices [ 8 ];
	D3DXMATRIXA16 mWorldMatrix;

	D3DXVECTOR3 mWorldSpaceFrustumVertices [ 8 ];


public:
//	CFrustum(float ZNear, float ZFar, float FOV, float AspectRatio);
	virtual ~CFrustum(void);
	void UpdateParams ( float zNear, float zFar, float fov, float aspectRatio);

	//override in derived classes to render strips, lists etc.
	virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice) ;

	//override in derived classes to construct according to desired geometry
	virtual HRESULT Construct(LPDIRECT3DDEVICE9 pd3dDevice);
	void UpdateWorldMatrix ( D3DXMATRIXA16& WorldMatrix );

	void GetNearPlane ( D3DXVECTOR3 *pts, float *uvs );
	bool GetNearPlaneClipped ( D3DXVECTOR3 *pts, float *uvs, D3DXPLANE waterPlane );


protected:
	int substituteInEquation ( D3DXVECTOR3 vec, D3DXPLANE plane);
	D3DXVECTOR3 IntersectEdgeWithPlane ( D3DXVECTOR3 pt0, D3DXVECTOR3 pt1, D3DXPLANE plane );


};

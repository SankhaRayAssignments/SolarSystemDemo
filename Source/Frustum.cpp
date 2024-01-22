#include "dxut.h"
#include "Frustum.h"


CFrustum::~CFrustum(void)
{
//if ( mpVertexBuffer  ) mpVertexBuffer->Release();
	
	//if ( mpIndexBuffer )   mpIndexBuffer->Release();

//	free ( mFrustumVertices );
}

void CFrustum::UpdateParams ( float zNear, float zFar, float fov, float aspectRatio)
{
		HRESULT hr;
	////////////////////////////////////////////
	/////// Update the Paramters
	///////////////////////////////////////////

	mZNear			= zNear;
	mZFar			= zFar;
	mFOV			= fov;
	mAspectRatio	= aspectRatio;


	//////// Compute 8 Vertices	///////////////

	float hNear, hFar ;	// hn/hf = hfar/zf = tan ( fov /2 );
	float wNear, wFar ;	// wn/hf = wfar/wf = aspect ratio;

	hNear	= tan ( mFOV / 2 ) * mZNear;
	hFar	= tan ( mFOV / 2 ) * mZFar;

	wNear	= hNear * mAspectRatio ;
	wFar	= hFar  * mAspectRatio ;


	float zNBias = mZNear + 0.0001f;
	// Near Points
	mFrustumVertices [0] = D3DXVECTOR3 ( -wNear,  hNear, -zNBias ) ;
	mFrustumVertices [1] = D3DXVECTOR3 ( -wNear, -hNear, -zNBias ) ;
	mFrustumVertices [2] = D3DXVECTOR3 (  wNear, -hNear, -zNBias ) ;
	mFrustumVertices [3] = D3DXVECTOR3 (  wNear,  hNear, -zNBias ) ;

	// Far Points
	mFrustumVertices [4] = D3DXVECTOR3 ( -wFar,  hFar, -mZFar ) ;
	mFrustumVertices [5] = D3DXVECTOR3 ( -wFar, -hFar, -mZFar ) ;
	mFrustumVertices [6] = D3DXVECTOR3 (  wFar, -hFar, -mZFar ) ;
	mFrustumVertices [7] = D3DXVECTOR3 (  wFar,  hFar, -mZFar ) ;

	////////////////////////////////////////////
	// Updating the Vertex Buffer
	////////////////////////////////////////////

	// Lock VB

	SPosCol *pvData = NULL;
	V( LockVB ( 0, (void**)&pvData, 0 ) );

	// Fill in Data
	int alpha = 255, r = 255, g = 255, b = 255;

	for ( int i = 0; i < 8; i++)
	{
		*pvData = SPosCol ( mFrustumVertices[i].x,mFrustumVertices[i].y,mFrustumVertices[i].z, D3DCOLOR_ARGB ( alpha, r, g, b ));
		pvData++;
	}

	// Unlock
	V (UnLockVB ());
	
}


HRESULT CFrustum::Render(LPDIRECT3DDEVICE9 pd3dDevice) 
{
	HRESULT hr;

	V_RETURN ( SetFVFVBIBOnDevice ( pd3dDevice ) );

	pd3dDevice->SetTransform( D3DTS_WORLD, &mWorldMatrix );
	pd3dDevice->SetRenderState ( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	pd3dDevice->DrawIndexedPrimitive ( D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12 );
	pd3dDevice->SetRenderState ( D3DRS_FILLMODE, D3DFILL_SOLID );
	return hr;
}


HRESULT CFrustum::Construct(LPDIRECT3DDEVICE9 pd3dDevice) 
{ 
	HRESULT hr;
	// Just Create the Vertex Buffer, It will be Updated in UpdateParams()

	CreateVertexBuffer ( pd3dDevice, sizeof ( SPosCol ), 8, D3DFVF_DIFFUSE | D3DFVF_XYZ, 
						D3DUSAGE_WRITEONLY, D3DPOOL_MANAGED );
	

	// Create Index Buffer
	USHORT frustumIndices[36] = {
								// Near
									0, 1, 2,
									0, 2, 3,
								// Left
									4, 5, 0,
									0, 5, 1,
								// Right
									3, 2, 7,
									7, 2, 6,						
								// Top
									4, 0, 7,
									7, 0, 3,
								// Bottom
									1, 5, 2,
									2, 5, 6,
								// Far
									5, 4, 6,
									4, 7, 6,
	};

								
	CreateIndexBuffer ( pd3dDevice, sizeof ( USHORT ), 36, D3DFMT_INDEX16, D3DUSAGE_WRITEONLY, D3DPOOL_MANAGED );

	// Lock Index Buffer
	USHORT *pIndexData = NULL;

	V_RETURN ( LockIB ( 0, (void**)&pIndexData, 0 ) );


	// Fill in Data
	memcpy ( pIndexData, frustumIndices, sizeof ( USHORT )* 36  );



	// Unlock IB
	V_RETURN ( UnLockIB () );
	
	D3DXMatrixIdentity ( &mWorldMatrix );

	return hr; 
}

void CFrustum::UpdateWorldMatrix ( D3DXMATRIXA16& WorldMatrix )
{
	D3DXMatrixInverse ( &mWorldMatrix, 0, &WorldMatrix );

	for (int i = 0 ; i < 8 ;i++)
	{
		D3DXVec3TransformCoord ( &mWorldSpaceFrustumVertices[i], &mFrustumVertices[i], &mWorldMatrix );
	}

}

void CFrustum::GetNearPlane ( D3DXVECTOR3 *pts, float *uvs )
{
	// Assuming that array has been created and passed in
	pts [0] = mWorldSpaceFrustumVertices[0];
	pts [1] = mWorldSpaceFrustumVertices[1];
	pts [2] = mWorldSpaceFrustumVertices[2];
	pts [3] = mWorldSpaceFrustumVertices[3];

	// Set U V
	uvs[0] = 0.0f; uvs[1] = 0.0f;
	uvs[2] = 0.0f; uvs[3] = 1.0f;
	uvs[4] = 1.0f; uvs[5] = 1.0f;
	uvs[6] = 1.0f; uvs[7] = 0.0f;

}

bool CFrustum::GetNearPlaneClipped ( D3DXVECTOR3 *pts, float *uvs, D3DXPLANE waterPlane )
{
	bool bFrustumCulling = false;
	D3DXVECTOR3 nearPPts [4 ];
	D3DXVECTOR3 nearPClippedPts [4 ];

	nearPPts [0] = mWorldSpaceFrustumVertices[0];
	nearPPts [1] = mWorldSpaceFrustumVertices[1];
	nearPPts [2] = mWorldSpaceFrustumVertices[2];
	nearPPts [3] = mWorldSpaceFrustumVertices[3];

	int currClipIndx = 0; 
	int originSign = substituteInEquation ( D3DXVECTOR3 (0, 0, 0 ), waterPlane );


	for (int i = 0; i < 4; i++)
	{
		int currentPoint =  i;
		int nextPoint	=  (i + 1 ) % 4;
		int currPtSign  = substituteInEquation ( nearPPts [ currentPoint ], waterPlane );
		int nextPtSign  = substituteInEquation ( nearPPts [ nextPoint ], waterPlane );

		if ( currPtSign == originSign )	// Current point on the side of Origin Include it
		{
			bFrustumCulling = true;
			// Include it
			nearPClippedPts [ currClipIndx ]  = nearPPts [ currentPoint ] ;
			currClipIndx++;
			if ( nextPtSign == originSign )	// the Next point is on the side of the origin
			{
				// This will be included in next iteration

			}
			else
			{
				// Insert and use that Point
				D3DXVECTOR3 tempPt = IntersectEdgeWithPlane ( nearPPts [ currentPoint ], nearPPts [ nextPoint ], waterPlane );
				nearPClippedPts [ currClipIndx ] = tempPt ;
				currClipIndx++;
				
			}

		}
		else	// Current point is on the Other Side Check The Next Point
		{
			if ( nextPtSign == originSign )	// the Next point is on the side of the origin
			{
				// Intersect and use the point 
				D3DXVECTOR3 tempPt = IntersectEdgeWithPlane ( nearPPts [ currentPoint ], nearPPts [ nextPoint ], waterPlane );
				nearPClippedPts [ currClipIndx ] = tempPt ;
				currClipIndx++;
			}
			else
			{
				// If Next point is on the other side IGNORE THE edge
			}
		}
	}

	pts[0] = nearPClippedPts [ 0 ];
	pts[1] = nearPClippedPts [ 1 ];
	pts[2] = nearPClippedPts [ 2 ];
	pts[3] = nearPClippedPts [ 3 ];
	// Set U V

	uvs[0] = 0.0f; uvs[1] = 0.0f;
	uvs[2] = 0.0f; uvs[3] = 1.0f;
	uvs[4] = 1.0f; uvs[5] = 1.0f;
	uvs[6] = 1.0f; uvs[7] = 0.0f;

	return bFrustumCulling;

}
int CFrustum::substituteInEquation ( D3DXVECTOR3 vec, D3DXPLANE plane)
{
	// If the point is on the plane return 0;
	// Otherwise will return -1/ +1 depending on the sign

	float res = plane.a * vec.x + plane.b * vec.y + plane.c * vec.z + plane.d;

	if ( fabs ( res ) < 0.001f )
		return 0;//POINT_ON_PLANE;
	else return ( res  > 0.0f ) ?  1:-1;//POINT_ON_POSITIVE_PLANE : POINT_ON_NEGETIVE_PLANE ;
}

D3DXVECTOR3 CFrustum::IntersectEdgeWithPlane ( D3DXVECTOR3 pt0, D3DXVECTOR3 pt1, D3DXPLANE plane )
{
	D3DXVECTOR3 ptOfIntersection ;
	D3DXPlaneIntersectLine ( &ptOfIntersection, &plane, &pt0, &pt1 );
	return ptOfIntersection ;
}
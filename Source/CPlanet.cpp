#include "dxut.h"
#include "CPlanet.h"

CPlanet::CPlanet(CSphere *Mesh , float distance, float rev_time , float rot_time , float scale,  string texName  ) 
				: CSphereObject ( Mesh, scale, texName)
{
	mRevolutionTime = rev_time;
	mRotationTime = rot_time;
	mDistance = distance;


	mCurrRotAngle = 0.0f;
	mCurrRevAngle = 0.0f;
	D3DXMatrixIdentity ( &mParentWorldPos );
	mOrbit = new COrbit ( mDistance );
	mfTilt = 0.0f;
}

CPlanet::~CPlanet(void)
{
	SAFE_DELETE ( mOrbit );
}


void CPlanet::SetParentWorldMat ( D3DXMATRIXA16& matIn )
{
	mParentWorldPos = matIn ;
}
D3DXVECTOR3 CPlanet::GetCenterCoord ()
{
	return D3DXVECTOR3 ( 0, 0, 0 );
}

HRESULT CPlanet::Construct(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr;
	V_RETURN ( CSphereObject::Construct ( pd3dDevice ));
	V_RETURN ( mOrbit->Construct (pd3dDevice ));
	return hr;
}
HRESULT CPlanet::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr;

	D3DXMATRIXA16 worldMat;


	V_RETURN ( CSphereObject::Render ( pd3dDevice ));

	worldMat = mParentWorldPos ;
	pd3dDevice->SetTransform ( D3DTS_WORLD, &worldMat );
	V_RETURN ( mOrbit->Render (pd3dDevice ));
	
	return hr;
}

void CPlanet::UpdatePosition ( float fElapsedTime )//, D3DXMATRIXA16& parentWorldMat )
{

	// Unifrom Transfrom

	float dRot = 360.0f / ( TIMEFACTOR * mRotationTime );
	mCurrRotAngle += ( dRot * fElapsedTime );

	while ( mCurrRotAngle >= 360.0f ) 	mCurrRotAngle -= 360.0f;

	D3DXMATRIXA16 ScaleMat;
	D3DXMATRIXA16 RotMat;
	D3DXMATRIXA16 TransMat;
	D3DXMATRIXA16 RevMat;

	// Scale
	D3DXMatrixIdentity ( &ScaleMat );
	D3DXMatrixScaling ( &ScaleMat, mScale, mScale, mScale );


	D3DXMATRIXA16 tiltMat;
	D3DXVECTOR3 upVec;
	

	if ( mfTilt != 0.0f )
	{
		D3DXMatrixRotationX ( &tiltMat, D3DXToRadian ( mfTilt ));
		D3DXVec3TransformCoord ( &upVec, &D3DXVECTOR3(0,1,0), &tiltMat );
	}

	// Rotation
	//D3DXMatrixRotationAxis ( &RotMat, &upVec, D3DXToRadian ( mCurrRotAngle  ) );


	// Rotation
	D3DXMatrixRotationY ( &RotMat, D3DXToRadian ( mCurrRotAngle  ) );

//	 mOrbitCoord

	// Translation
	D3DXMatrixTranslation ( &TransMat, mDistance, 0, 0 );

	// Revolution
	float dAngle = 360.0f / ( TIMEFACTOR * mRevolutionTime );
	mCurrRevAngle +=  ( dAngle * fElapsedTime );
	while ( mCurrRevAngle >= 360.0f )
		mCurrRevAngle -= 360.0f;

	D3DXMatrixRotationY( &RevMat, D3DXToRadian ( mCurrRevAngle ) );


	mWorldWithOutScale = TransMat * RevMat * mParentWorldPos;
	mWorldTransform =  ScaleMat * RotMat * mWorldWithOutScale ;

	
/*
	float dAngle = 360.0f / ( TIMEFACTOR * mRotationTime );
	D3DXMatrixRotationAxis ( &mRevMat, &mOrbitAxis,   D3DXMatrixRotationAxis );

	D3DXVECTOR4 temp;
	D3DXVec3Transform ( &temp, &mOrbitCoordinate, &mRevMat );

	mOrbitCoordinate.x	 = temp.x;
	mOrbitCoordinate.y	 = temp.y;
	mOrbitCoordinate.z	 = temp.z;

	mAbsoluteCoord = mCenterOfOrbit + mOrbitCoordinate;



	D3DXMATRIXA16 RotMat, ScaleMat;

	D3DXMatrixRotationAxis ( &RotMat, &mOrbitAxis, D3DXToRadian ( dRot ));
	D3DXMatrixIdentity ( &mWorldTransform );
	D3DXMatrixScaling ( &ScaleMat, mScale, mScale, mScale );

	mWorldTransform._41 = mAbsoluteCoord.x;
	mWorldTransform._42 = mAbsoluteCoord.y;
	mWorldTransform._43 = mAbsoluteCoord.z;
	*/

}
/*
HRESULT CPlanet::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr;

	pd3dDevice->SetTransform( D3DTS_WORLD, &mWorldTransform);

	pd3dDevice->SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );
	V_RETURN ( mpMesh->DrawIndexedTriStrip ( pd3dDevice, 0, 0, 0 ) );
	pd3dDevice->SetRenderState ( D3DRS_CULLMODE, D3DCULL_CW );
	return hr; 

}
*/





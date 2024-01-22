#include "DXUT.h"
#include "CObject.h"

CObject::CObject()
{
	mpMesh = 0;
	D3DXMatrixIdentity(&mWorldTransform);
}

void CObject::GetWorldTransform(float x, float y, float z, float rx, float ry, float rz)
{
	//given the position (x,y,z) and rotations along xyz axis (rx,ry,rz), get the worldtransform
	D3DXMATRIXA16 rotX, rotY, rotZ, rotMat;
	D3DXVECTOR3 unitX = D3DXVECTOR3(1,0,0);
	D3DXVECTOR3 unitY = D3DXVECTOR3(0,1,0);
	D3DXVECTOR3 unitZ = D3DXVECTOR3(0,0,1);

	D3DXMatrixRotationAxis(&rotX, &unitX, rx); 
	D3DXMatrixRotationAxis(&rotY, &unitY, ry); 
	D3DXMatrixRotationAxis(&rotZ, &unitZ, rz); 

	rotMat = rotX;
	D3DXMatrixMultiply(&rotMat,&rotMat,&rotY);
	D3DXMatrixMultiply(&rotMat,&rotMat,&rotZ);

	mWorldTransform = rotMat;

	mWorldTransform._41 = x;
	mWorldTransform._42 = y;
	mWorldTransform._43 = z;

}

CObject::CObject(float x, float y, float z, float rx, float ry, float rz)
{
	GetWorldTransform(x,y,z,rx,ry,rz);
}


CObject::~CObject()
{
	//object only holds a pointer to comman instanced mesh
	//it does not own it, (game/renderer does), hence should not delete it
	//as the same mesh may be use by other objects.
}

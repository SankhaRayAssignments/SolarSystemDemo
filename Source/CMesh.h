#pragma once
#include <d3d9.h>


struct SPosCol
{
	float x, y, z;
	ULONG col;
public:

	SPosCol(float xin = 0, float yin = 0, float zin = 0, ULONG colin = 0) 
	{
		x = xin; y = yin; z = zin; col = colin;
	}
	~SPosCol() 
	{
	}
};
struct SPosNormal 
{
	float x, y, z;
	D3DXVECTOR3 Normal;

	SPosNormal ( float xin, float yin , float zin , D3DXVECTOR3 norm)// = D3DXVECTOR3 (0,0,0))  
	{
		x = xin; y = yin; z = zin; 
		Normal = norm;
	}
};
struct SPosNormalUV
{
	float x, y, z;
	D3DXVECTOR3 Normal;
	float u, v;

	SPosNormalUV ( float xin, float yin , float zin , D3DXVECTOR3 norm, float uin, float vin)// = D3DXVECTOR3 (0,0,0))  
	{
		x = xin; y = yin; z = zin; 
		Normal = norm;
		u = uin;
		v = vin;
	}
};

struct SPosNormal2Tex
{
	float x, y, z;
	D3DXVECTOR3 Normal;
	float u0, v0;
	float u1, v1;

	SPosNormal2Tex ( float xin = 0, float yin = 0, float zin = 0 , D3DXVECTOR3 norm  = D3DXVECTOR3(0,0,1), float u0 = 0, float v0 = 0, float u1 = 0, float v1 = 0)// = D3DXVECTOR3 (0,0,0))  
	{
		x = xin; y = yin; z = zin; 
		Normal = norm;
		this->u0 = u0;
		this->v0 = v0;
		this->u1 = u1;
		this->v1 = v1;
	}
};


struct SPosColTex
{
	float x, y, z;
	ULONG col;
	float u, v;
public:

	SPosColTex(float xin = 0, float yin = 0, float zin = 0, ULONG colin = 0, float uin = 0, float vin = 0) 
	{
		x = xin; y = yin; z = zin; col = colin; u  = uin; v = vin;
	}
};

class CMesh 
{
//funcs
public:
	CMesh();
	virtual ~CMesh();

	HRESULT CreateVertexBuffer(LPDIRECT3DDEVICE9 pD3DDevice, int szVert, int numVert, DWORD fvf, 
		ULONG usage, D3DPOOL pool);
	HRESULT LockVB(int offset, void** ppVBdata, DWORD flags = 0);
	HRESULT UnLockVB();

	HRESULT CreateIndexBuffer(LPDIRECT3DDEVICE9 pD3DDevice, int szIndx, int numIndices, 
		D3DFORMAT format, ULONG usage, D3DPOOL pool);
	HRESULT LockIB(int offset, void** ppIBdata, DWORD flags = 0);
	HRESULT UnLockIB();

	HRESULT SetFVF(LPDIRECT3DDEVICE9 pD3DDevice);
	HRESULT SetStreamSource(LPDIRECT3DDEVICE9 pD3DDevice, UINT streamNumber,UINT offsetInBytes);
	HRESULT SetIndices(LPDIRECT3DDEVICE9 pD3DDevice);
	HRESULT DrawIndexedTriStrip(LPDIRECT3DDEVICE9 pD3DDevice, int baseVertIndex, int minVertIndex,
		int startIndex);
	HRESULT DrawIndexedLineList(LPDIRECT3DDEVICE9 pD3DDevice, int baseVertIndex, int minVertIndex,
		int startIndex);


	HRESULT SetFVFVBIBOnDevice(LPDIRECT3DDEVICE9 pD3DDevice);

	//override in derived classes to render strips, lists etc.
	virtual HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice) { return S_OK;}

	//override in derived classes to construct according to desired geometry
	virtual HRESULT Construct(LPDIRECT3DDEVICE9 pd3dDevice) { return S_OK; }
	virtual	void GetMin(D3DXVECTOR3& minIn) {}
	virtual void GetMax(D3DXVECTOR3& maxIn) {}
	virtual void SetMinMax(D3DXVECTOR3& minIn, D3DXVECTOR3& maxIn) {}


//vars
protected:
	int mSizeOfVertex;
	int mNumVertices;
	DWORD mFVF;
	ULONG mVBUsage;
	D3DPOOL mVBPool;
	LPDIRECT3DVERTEXBUFFER9 mpVertexBuffer; 

	int mSizeOfIndex;
	int mNumIndices;
	ULONG mIBUsage;
	D3DPOOL mIBPool;
	D3DFORMAT mIBFormat;
	LPDIRECT3DINDEXBUFFER9  mpIndexBuffer;



};
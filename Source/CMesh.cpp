#include "DXUT.h"
#include "CMesh.h"

CMesh::CMesh()
{
	mpVertexBuffer = NULL;
	mpIndexBuffer = NULL;
}

CMesh::~CMesh()
{
	if ( mpVertexBuffer  ) mpVertexBuffer->Release();
	
	if ( mpIndexBuffer )   mpIndexBuffer->Release();
}

HRESULT CMesh::CreateVertexBuffer(LPDIRECT3DDEVICE9 pD3DDevice, int szVert, int numVert, 
								  DWORD fvf, ULONG usage, D3DPOOL pool)
{
	HRESULT hr;

	mSizeOfVertex = szVert;
	mNumVertices = numVert;
	mFVF = fvf;
	mVBPool = pool;
	mVBUsage = usage;

	hr = pD3DDevice->CreateVertexBuffer(mSizeOfVertex*mNumVertices, mVBUsage, mFVF, mVBPool,
					&mpVertexBuffer, NULL);

	return hr;
}

HRESULT CMesh::LockVB(int offset, void** ppVBdata, DWORD flags /*= 0*/)
{
	return mpVertexBuffer->Lock(offset, mSizeOfVertex*mNumVertices, ppVBdata, flags);
}

HRESULT CMesh::UnLockVB()
{
	return mpVertexBuffer->Unlock();
}

HRESULT CMesh::CreateIndexBuffer(LPDIRECT3DDEVICE9 pD3DDevice, int szIndx, int numIndices, 
								  D3DFORMAT format, ULONG usage, D3DPOOL pool)

{
	HRESULT hr;

	mSizeOfIndex = szIndx;
	mNumIndices = numIndices;
	mIBPool = pool;
	mIBUsage = usage;
	mIBFormat = format;

	hr = pD3DDevice->CreateIndexBuffer(mSizeOfIndex*mNumIndices, mIBUsage, mIBFormat, mIBPool,
		&mpIndexBuffer, NULL);

	return hr;
}

HRESULT CMesh::LockIB(int offset, void** ppIBdata, DWORD flags /*= 0*/)
{
	return mpIndexBuffer->Lock(offset, mSizeOfIndex*mNumIndices, ppIBdata, flags);
}

HRESULT CMesh::UnLockIB()
{
	return mpIndexBuffer->Unlock();
}

HRESULT  CMesh::SetFVF(LPDIRECT3DDEVICE9 pD3DDevice)
{
	return pD3DDevice->SetFVF(mFVF);
}

HRESULT CMesh::SetStreamSource(LPDIRECT3DDEVICE9 pD3DDevice, UINT streamNumber,UINT offsetInBytes) 
{
    return pD3DDevice->SetStreamSource( streamNumber, mpVertexBuffer, 
		offsetInBytes, mSizeOfVertex );
}

HRESULT CMesh::SetIndices(LPDIRECT3DDEVICE9 pD3DDevice)
{
	return pD3DDevice->SetIndices(mpIndexBuffer);
}

HRESULT CMesh::SetFVFVBIBOnDevice(LPDIRECT3DDEVICE9 pD3DDevice)
{
	HRESULT hr = S_OK;

	// Setup our vertex FVF code
	V_RETURN(SetFVF(pD3DDevice));

	V_RETURN(SetStreamSource(pD3DDevice, 0, 0));

	V_RETURN(SetIndices(pD3DDevice));

	return hr;
}


HRESULT CMesh::DrawIndexedTriStrip(LPDIRECT3DDEVICE9 pD3DDevice, int baseVertIndex, int minVertIndex,
								   int startIndex)
{
    return pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, baseVertIndex, minVertIndex, 
		mNumVertices, startIndex, (mNumIndices-2) );
}

HRESULT CMesh::DrawIndexedLineList(LPDIRECT3DDEVICE9 pD3DDevice, int baseVertIndex, int minVertIndex,
								   int startIndex)
{
    return pD3DDevice->DrawIndexedPrimitive( D3DPT_LINELIST, baseVertIndex, minVertIndex, 
		mNumVertices, startIndex, mNumIndices/2 );

}
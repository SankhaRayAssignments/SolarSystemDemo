#include "DXUT.h"
#include "CTerrain.h"

CTerrain::CTerrain()
{
}

CTerrain::CTerrain(D3DXVECTOR3 min, D3DXVECTOR3 max, int rows, int cols, string fname, float DetailTexScale, string BaseTextName, string DetailTexName)
{
	mMin = min;
	mMax = max;
	mRows = rows;
	mCols = cols;

	mHeightMapName = fname;
	mpHeightMap = 0;
	mpHmapData = 0;

	//mLightDir = D3DXVECTOR3(0.57735f,0.57735f,0.57735f);
	mDetailTexScale = DetailTexScale;
	mBaseTex = NULL;
	mDetailTex = NULL;
	mBaseTexName = BaseTextName ;
	mDetailTexName = DetailTexName;

	bDetailTextureOnOff = true;
}


CTerrain::~CTerrain()
{
	delete mpHeightMap;
	
	if ( mBaseTex )	mBaseTex->Release();

	if ( mDetailTex  ) mDetailTex->Release();
}

HRESULT CTerrain::Render(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HRESULT hr = S_OK;

	// Set Up Trilinear Filtering
	// For tex 0
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState ( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );

	// fOR tex 1
	pd3dDevice->SetSamplerState ( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState ( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState ( 1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );


	// base tex on Stage 0

	pd3dDevice->SetTextureStageState ( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState ( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
	pd3dDevice->SetTextureStageState ( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE );


	// Deteial tex on Stage 1
	pd3dDevice->SetTextureStageState ( 1, D3DTSS_COLOROP, D3DTOP_ADDSIGNED );
	pd3dDevice->SetTextureStageState ( 1, D3DTSS_COLORARG1, D3DTA_CURRENT );
	pd3dDevice->SetTextureStageState ( 1, D3DTSS_COLORARG2, D3DTA_TEXTURE );

	// Set the texture

	pd3dDevice->SetTexture ( 0, mBaseTex );

	if ( bDetailTextureOnOff )
		pd3dDevice->SetTexture ( 1, mDetailTex );
	else pd3dDevice->SetTexture ( 1, NULL );

	// Draw
	V_RETURN(DrawIndexedTriStrip(pd3dDevice,0,0,0));

	// Set the textures to NULL/Clear
	pd3dDevice->SetTexture ( 0, NULL );
	pd3dDevice->SetTexture ( 1, NULL );

	return hr;

}

float CTerrain::GetHeightVal(int row, int col)
{
	//assuming rgb tex, looks like for tga origin is at bottom left


	float heightVal = mpHmapData[3*((mRows-row-1)*mCols + col)+0]; 
	float r = (heightVal/255.0f);
	float hValReMapped = (1.0f -r)*mMin.y + r*mMax.y;

	return hValReMapped;

}

void CTerrain::GetRowColGivenWorldPos(D3DXVECTOR3& vecIn, float& row, float& col)
{
	//given a position in the world space, what is the row/col of the point in XZplane
	row = (vecIn.z - mMin.z) / mDelZ;
	col = (vecIn.x - mMin.x) / mDelX;
}

float CTerrain::GetHeightValInterpolated(float row, float col)
{
	//note the algorithm used here has a problem when we go to the borders of the
	//terrain.. can fix this by collision not allow player to get till there..

	//note that the row and col values are floats
	
	//define two vectors vec1 = normalize(pt-lowerleft corner of rect)
	// vec2 = normal (upper right corner of rect - lowerleft corner of rect)
	D3DXVECTOR3 lowerLeft	= D3DXVECTOR3((int(col))*mDelX, 0, (int(row)+1)*mDelZ);
	D3DXVECTOR3 upperRight	= D3DXVECTOR3((int(col)+1)*mDelX, 0, (int(row))*mDelZ);
	D3DXVECTOR3 currPos     = D3DXVECTOR3(col*mDelX, 0, row*mDelZ);

	D3DXVECTOR3 vec1		= currPos - lowerLeft;
	float vec1Len = D3DXVec3Length(&vec1);
	D3DXVec3Normalize(&vec1, &vec1);

	D3DXVECTOR3 vec2		= upperRight - lowerLeft;
	float vec2Len = D3DXVec3Length(&vec2);
	D3DXVec3Normalize(&vec2, &vec2);

	lowerLeft.y = GetHeightVal((int(row)+1), int(col));

	upperRight.y = GetHeightVal(int(row), (int(col)+1));

	float hVal = 0.0f;
	//check1 - dot vec1 and vec2 > 0.95? if so 
	if( D3DXVec3Dot(&vec1,&vec2) > 0.95f )
	{
		//desired point is on the line interpolate height accordingly

		float r = vec1Len/vec2Len;

		hVal =  (1.0f-r)*lowerLeft.y + r*upperRight.y;

//		return hVal;
	}
	else //if not use cross product to determine if it is in upper triangle or lower
	{
		D3DXVECTOR3 temp;
		D3DXVec3Cross(&temp, &vec2, &vec1);

		D3DXVECTOR3 pt1, pt2, pt3;

		if(temp.y > 0) //point is in upper triangle
		{
			pt1 = lowerLeft;
			pt2 = upperRight;
			pt3 = D3DXVECTOR3((int(col))*mDelX, GetHeightVal(int(row), int(col)), (int(row))*mDelZ);
		}
		else //point is in lower triangle
		{
			pt1 = lowerLeft;
			pt2 = D3DXVECTOR3((int(col)+1)*mDelX, GetHeightVal((int(row)+1), (int(col)+1)), (int(row)+1)*mDelZ);
			pt3 = upperRight;
		}

		//get the plane equation, get the z of desired point from that and return
		//ax+by+cz+d = 0

		//get the n = (a,b,c)
		D3DXVECTOR3 temp1 = pt2 - pt1;
		D3DXVECTOR3 temp2 = pt3 - pt1;
		D3DXVECTOR3 plnNormal;
		D3DXVec3Cross(&plnNormal, &temp1, &temp2);
		D3DXVec3Normalize(&plnNormal, &plnNormal);

		// d = -(a*x1+b*y1+c*z1)
		float d = -((plnNormal.x * pt1.x) + (plnNormal.y * pt1.y) + (plnNormal.z * pt1.z));

		//so given ax+by+cz+d = 0, given x,z, required y is y = -(ax+cz+d)/b
		hVal = -(plnNormal.x*currPos.x + plnNormal.z*currPos.z + d)/(plnNormal.y);

//		return hVal;

	}
	return hVal;

	//return (mMin.y-2); //show not come here!!
}

D3DXVECTOR3 CTerrain::GetNormal(int row, int col)
{
	if((row == (mRows-1)) || (col == (mCols - 1)))
		return D3DXVECTOR3(0, 1, 0);	// Up Vector

	float h		= GetHeightVal(row,col);
	float hleft	= GetHeightVal(row, (col+1));
	float hdown	= GetHeightVal((row+1), col);

	D3DXVECTOR3 v		= D3DXVECTOR3(mMin.x + mDelX*col, h, mMin.z + mDelZ*row);
	D3DXVECTOR3 vleft	= D3DXVECTOR3(mMin.x + mDelX*(col+1), hleft, mMin.z + mDelZ*row);
	D3DXVECTOR3 vdown	= D3DXVECTOR3(mMin.x + mDelX*col, hdown, mMin.z + mDelZ*(row+1));

	D3DXVECTOR3 v1 = vleft - v;
	D3DXVECTOR3 v2 = vdown - v;

	D3DXVECTOR3 vn;
	D3DXVec3Cross(&vn, &v2, &v1);
	D3DXVec3Normalize(&vn, &vn);

	return vn;
}


HRESULT CTerrain::Construct(LPDIRECT3DDEVICE9 pd3dDevice )
{
	HRESULT hr = S_OK;

	mpHeightMap = new CTGA(mHeightMapName);
	mpHeightMap->LoadTGA();

	mpHmapData = mpHeightMap->GetData();

	mDelX = (mMax.x - mMin.x)/(mCols-1);
	mDelZ = (mMax.z - mMin.z)/(mRows-1);

	float invColsMin1 = 1.0f / ( mCols - 1 );		// Delta U
	float invRowsMin1 = 1.0f / ( mRows - 1 );		// Delta V


	//vertex data
	int numVerts = mRows*mCols;
	SPosNormal2Tex* terrVerts =  (SPosNormal2Tex*)malloc(sizeof(SPosNormal2Tex)*numVerts); //new SPosCol[numVerts];

	for(int i = 0; i < mRows; i++)
	{
		for(int j = 0; j < mCols; j++)
		{
			float hValReMap = GetHeightVal(i, j);

			D3DXVECTOR3 normal = GetNormal(i, j); 

//			float ndotl = max( D3DXVec3Dot(&normal, &mLightDir), 0);

			terrVerts[i*mCols + j].x = mMin.x + mDelX*j;
			terrVerts[i*mCols + j].y = hValReMap;
			terrVerts[i*mCols + j].z = mMin.z + mDelZ*i;
			terrVerts[i*mCols + j].Normal = normal;	//D3DCOLOR_ARGB(125, UINT( ndotl * 255) , UINT( ndotl * 255), UINT( ndotl * 255));
//			terrVerts[i*mCols + j].Normal =  D3DXVECTOR3(UINT( ndotl * 255) , UINT( ndotl * 255), UINT( ndotl * 255));
			terrVerts[i*mCols + j].u0 = j * invColsMin1;						// Base Texture U,V
			terrVerts[i*mCols + j].v0 = i * invRowsMin1;
			terrVerts[i*mCols + j].u1 = mDetailTexScale * j * invColsMin1;		// Details Texture U,V
			terrVerts[i*mCols + j].v1 = mDetailTexScale * i * invRowsMin1;
		}
	}


	//index data
	int numIndices = (mRows-1)*((mCols)*2 + 2);
	USHORT* terrIndices = (USHORT*) malloc(sizeof(USHORT)*numIndices); //new USHORT[numIndices];

	int k = 0;
	for(int i = 0; i < (mRows-1); i++)
	{
		int j;
		for(j=0; j< mCols; j++)
		{
			//add (i,j), ((i+1),j), 
			terrIndices[k++] = (USHORT)(i*mCols + j); 
			terrIndices[k++] = (USHORT)((i+1)*mCols + j); 
		}

		//for degenerate triangles
		terrIndices[k++] =  (USHORT)((i+1)*mCols + (j-1)); 
		terrIndices[k++] = (USHORT)((i+1)*mCols + (0)); 
	}


	//VB and IB
	V_RETURN(CreateVertexBuffer(pd3dDevice, sizeof(SPosNormal2Tex), numVerts, D3DFVF_XYZ |D3DFVF_NORMAL | D3DFVF_TEX2, 
		D3DUSAGE_WRITEONLY, D3DPOOL_MANAGED));
	
	
	SPosNormal2Tex* pVdata = NULL;

	V_RETURN(LockVB(0, (void**)&pVdata));

	memcpy(pVdata,terrVerts,sizeof(SPosNormal2Tex)*numVerts);

	V_RETURN(UnLockVB());

	V_RETURN(CreateIndexBuffer(pd3dDevice, sizeof(USHORT), numIndices, D3DFMT_INDEX16,
		D3DUSAGE_WRITEONLY, D3DPOOL_MANAGED));

	USHORT* pIData = NULL;

	V_RETURN(LockIB(0, (void**)&pIData));

	memcpy(pIData,terrIndices, sizeof(USHORT)*numIndices);

	V_RETURN(UnLockIB());

	free(terrVerts);
	free(terrIndices);

	// Create The Textures

	TCHAR temp[200];
	mbstowcs ( temp, mBaseTexName.data(), 200 );
	V_RETURN ( D3DXCreateTextureFromFile ( pd3dDevice, temp, &mBaseTex ) );
	
	memset ( temp, 0, sizeof(TCHAR)*200 );
	mbstowcs ( temp, mDetailTexName.data(), 200 );
	V_RETURN ( D3DXCreateTextureFromFile ( pd3dDevice, temp, &mDetailTex ) );


	return hr;
}

void CTerrain::SetDeatilTextureOnOff ( bool DetailTextureOnOff )
{
	bDetailTextureOnOff = DetailTextureOnOff ;
}

/*
bool CTerrain::isBasePointInsideTerrain ( const D3DXVECTOR3 pt1,const D3DXVECTOR3 pt2,const D3DXVECTOR3 pt3,const D3DXVECTOR3 pt4 )
{
	if ( 
		// Check X Coord
		 (mMax.x >= pt1.x && pt1.x >= mMin.x ) &&
		 (mMax.x >= pt2.x && pt2.x >= mMin.x ) &&
		 (mMax.x >= pt3.x && pt3.x >= mMin.x ) &&
		 (mMax.x >= pt4.x && pt4.x >= mMin.x ) &&
		 // Check Z coord
		 (mMax.z >= pt1.z && pt1.z >= mMin.z ) &&
		 (mMax.z >= pt2.z && pt2.z >= mMin.z ) &&
		 (mMax.z >= pt3.z && pt3.z >= mMin.z ) &&
		 (mMax.z >= pt4.z && pt4.z >= mMin.z ) )
			return true;
	else return false;
}	
*/
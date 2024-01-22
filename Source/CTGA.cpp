#include "DXUT.h"
#include "CTGA.h"
#include <fstream>
using std::ifstream;


CTGA::~CTGA()
{
	if(mpImageData)
		delete mpImageData;

}

HRESULT CTGA::LoadTGA()
{
	TGAHeader tHdr;

	ifstream file(mFileName.data(), std::ios_base::binary);

	if( !file.is_open() )
		return S_FALSE;

	if( !file.read( (char*)&tHdr, sizeof(TGAHeader) ) )
		return S_FALSE;

	if( tHdr.mImageType != 2)
		return S_FALSE;

	mWidth = tHdr.mWidth;
	mHeight = tHdr.mHeight;
	mBpp = tHdr.mPixelDepth;

	if ( (mWidth <= 0) || (mHeight <= 0) || ((mBpp !=24) && (mBpp != 32)) )
		return S_FALSE;

	UINT bytesPP = mBpp/8;
	UINT imgSize = mWidth * mHeight * bytesPP;

	mpImageData = new UCHAR[imgSize];

	if(mpImageData == NULL)
		return S_FALSE;

	if( !file.read((char*)mpImageData, imgSize) )
	{
		delete mpImageData;
		return S_FALSE;
	}

	//BGR -> RGB
	for(UINT i = 0; i < imgSize; i += bytesPP)
	{
		UCHAR tempB = mpImageData[i + 0];
		UCHAR tempR = mpImageData[i + 2];

		mpImageData[i + 0] = tempR;
		mpImageData[i + 2] = tempB;
	}

	return S_OK;
}

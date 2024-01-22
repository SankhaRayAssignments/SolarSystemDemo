#pragma once
#include "string"
using std::string;

struct TGAHeader
{
	unsigned char mIDLength;
	unsigned char mColorMapType;
	unsigned char mImageType;
	unsigned char mColorMapSpecification[5];
	USHORT	mXOrigin;
	USHORT mYOrigin;
	USHORT mWidth;
	USHORT mHeight;
	unsigned char mPixelDepth;
};

class CTGA
{

protected:
	string mFileName;
	UCHAR*	mpImageData;
	unsigned int	mBpp;
	unsigned int	mWidth, mHeight;

public:
	CTGA() {}
	CTGA(string fName) { mFileName = fName; }
	UCHAR* GetData() {return mpImageData; }
	~CTGA();
	HRESULT LoadTGA();

};
#ifndef CRIMAGEDATABLOCK_H
#define CRIMAGEDATABLOCK_H

#include "CrDefines.h"

#ifdef CR_SDK_EXPORTS
#define SCRSDK_API __declspec(dllexport)
#else
#define SCRSDK_API __declspec(dllimport)
#endif

namespace SCRSDK
{

class SCRSDK_API CrImageInfo
{
public:
	CrImageInfo();
	~CrImageInfo();

	CrInt32u GetBufferSize();

private:
	CrInt32u width;
	CrInt32u height;
	CrInt32u bufferSize;
};

class SCRSDK_API CrImageDataBlock
{
public:
	CrImageDataBlock();
	~CrImageDataBlock();

	void SetFrameNo(CrInt32u frame);
	CrInt32u GetFrameNo();

	void SetSize(CrInt32u size);
	CrInt32u GetSize();

	void SetData(CrInt8u* data);

	CrInt32u GetImageSize();
	CrInt8u* GetImageData();

private:
	CrInt32u frameNo;
	//Buffer size (byte) of pData. 
	CrInt32u size;
	//Buffer to store the image body. 
	CrInt8u* pData;

};

}
#endif //CRIMAGEDATABLOCK_H
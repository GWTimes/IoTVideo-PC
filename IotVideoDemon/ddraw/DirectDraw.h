
#ifndef DirectDraw_H
#define DirectDraw_H

//////////////////////////////////////////////////////////////////////////

#include <Windows.h>

//////////////////////////////////////////////////////////////////////////

enum
{
	IMAGE_FORMAT_YV12, 
	IMAGE_FORMAT_I420, 
	IMAGE_FORMAT_YUYV, 
	IMAGE_FORMAT_YVYU, 
	IMAGE_FORMAT_UYVY, 
	IMAGE_FORMAT_NV12, 
	IMAGE_FORMAT_NV21, 

	IMAGE_FORMAT_PAL8, 
	IMAGE_FORMAT_RGB555, 
	IMAGE_FORMAT_RGB565, 
	IMAGE_FORMAT_BGR24, 
	IMAGE_FORMAT_RGB32, 
	IMAGE_FORMAT_BGR32, 
};

//////////////////////////////////////////////////////////////////////////

struct ImageProperties
{
	DWORD dwImageFormat;
	DWORD dwWidth;
	DWORD dwHeight;
	DWORD dwVideoType;

	union
	{
		struct
		{
			LPBYTE lpY;
			LPBYTE lpU;
			LPBYTE lpV;
			LPBYTE lpReserve;
		};
		struct
		{
			LPBYTE lpY;
			LPBYTE lpUV;
		};
		struct
		{
			LPBYTE lpY;
			LPBYTE lpVU;
		};

		LPBYTE lpYUY2;
		LPBYTE lpYVYU;
		LPBYTE lpUYVY;
		LPBYTE lpRGB;
	};
};

//////////////////////////////////////////////////////////////////////////

DWORD DD_Initialize(LPHANDLE lpHandle);
DWORD DD_Draw(HANDLE hHandle, HWND hWnd, const ImageProperties *pImageProperties);
DWORD DD_Uninitialize(HANDLE hHandle);
void DD_RegisterDrawFun(HANDLE hHandle, void* DrawFun, void* user);

//////////////////////////////////////////////////////////////////////////

#endif

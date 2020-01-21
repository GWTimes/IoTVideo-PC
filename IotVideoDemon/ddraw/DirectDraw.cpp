
//#include "stdafx.h"
#include <atlcoll.h>
#include "DirectDraw.h"

#include "DirectDrawT.h"

#include "Utility.h"
#include "CriticalSection.h"

//////////////////////////////////////////////////////////////////////////

// BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID lpReserved)
// {
// 	switch (dwReasonForCall) {
// 		case DLL_PROCESS_ATTACH:
// 		case DLL_THREAD_ATTACH:
// 		case DLL_THREAD_DETACH:
// 		case DLL_PROCESS_DETACH:
// 			break;
// 	}
//     return TRUE;
// }

//////////////////////////////////////////////////////////////////////////

class CDirectDrawEx : public CDirectDraw
{
public:
	CDirectDrawEx()
		:	m_dwImageFormat(IMAGE_FORMAT_YV12), 
			m_dwWidth(0), 
			m_dwHeight(0), 
			m_hWnd(NULL)
	{
	}

	HRESULT Rebuild(HWND hWnd, const ImageProperties &imageProperties)
	{
		DWORD dwImageFormat = imageProperties.dwImageFormat;
		DWORD dwWidth	= imageProperties.dwWidth;
		DWORD dwHeight	= imageProperties.dwHeight;

		HRESULT hr = S_OK;
		if ((hWnd != m_hWnd) || (dwImageFormat != m_dwImageFormat) || (dwWidth != m_dwWidth) || (dwHeight != m_dwHeight)) {
			Release();

			DDSURFACEDESC2 ddsd = { 0 };
			switch (dwImageFormat) {
				case IMAGE_FORMAT_YV12:
					FormatYV12::Build(ddsd, dwWidth, dwHeight);
					hr = Create<FormatYV12>(hWnd, ddsd);
					break;
				case IMAGE_FORMAT_I420:
					FormatI420::Build(ddsd, dwWidth, dwHeight);
					hr = Create<FormatI420>(hWnd, ddsd);
					break;
				case IMAGE_FORMAT_YUYV:
					FormatYUY2::Build(ddsd, dwWidth, dwHeight);
					hr = Create<FormatYUY2>(hWnd, ddsd);
					break;
				case IMAGE_FORMAT_YVYU:
					FormatYVYU::Build(ddsd, dwWidth, dwHeight);
					hr = Create<FormatYVYU>(hWnd, ddsd);
					break;
				case IMAGE_FORMAT_UYVY:
					FormatUYVY::Build(ddsd, dwWidth, dwHeight);
					hr = Create<FormatUYVY>(hWnd, ddsd);
					break;
				case IMAGE_FORMAT_NV12:
					FormatNV12::Build(ddsd, dwWidth, dwHeight);
					hr = Create<FormatNV12>(hWnd, ddsd);
					break;

				case IMAGE_FORMAT_PAL8:
					FormatPAL8::Build(ddsd, dwWidth, dwHeight);
					hr = Create<FormatPAL8>(hWnd, ddsd);
					break;
				case IMAGE_FORMAT_RGB555:
					FormatRGB555::Build(ddsd, dwWidth, dwHeight);
					hr = Create<FormatRGB555>(hWnd, ddsd);
					break;
				case IMAGE_FORMAT_RGB565:
					FormatRGB565::Build(ddsd, dwWidth, dwHeight);
					hr = Create<FormatRGB565>(hWnd, ddsd);
					break;
				case IMAGE_FORMAT_BGR24:
					FormatBGR24::Build(ddsd, dwWidth, dwHeight);
					hr = Create<FormatBGR24>(hWnd, ddsd);
					break;
				case IMAGE_FORMAT_RGB32:
					FormatRGB32::Build(ddsd, dwWidth, dwHeight);
					hr = Create<FormatRGB32>(hWnd, ddsd);
					break;
				case IMAGE_FORMAT_BGR32:
					FormatBGR32::Build(ddsd, dwWidth, dwHeight);
					hr = Create<FormatBGR32>(hWnd, ddsd);
					break;
			}

			m_dwImageFormat	= dwImageFormat;
			m_dwWidth		= dwWidth;
			m_dwHeight		= dwHeight;
			m_hWnd			= hWnd;
		}

		return hr;
	}

	DWORD Draw(HWND hWnd, const ImageProperties &imageProperties)
	{
        //CGuard<CCriticalSection> guard(m_csDraw);

		if (Rebuild(hWnd, imageProperties) != S_OK) {
			return ERROR_INTERNAL_ERROR;
		}

		DWORD dwImageFormat = imageProperties.dwImageFormat;
		DWORD dwWidth	= imageProperties.dwWidth;
		DWORD dwHeight	= imageProperties.dwHeight;

		ImageSpace imageSpace = { 0 };
		switch (imageProperties.dwImageFormat) {
			case IMAGE_FORMAT_YV12:
			case IMAGE_FORMAT_I420:
				imageSpace.lpY = imageProperties.lpY;
				imageSpace.lpU = imageProperties.lpU;
				imageSpace.lpV = imageProperties.lpV;

				imageSpace.dwLineSize[0]= dwWidth;
				imageSpace.dwLineSize[1]= dwWidth >> 1;
				imageSpace.dwLineSize[2]= dwWidth >> 1;
				break;
			case IMAGE_FORMAT_YUYV:
			case IMAGE_FORMAT_YVYU:
			case IMAGE_FORMAT_UYVY:
				imageSpace.lpYUY2 = imageProperties.lpYUY2;

				imageSpace.dwLineSize[0]= dwWidth << 1;
				break;
			case IMAGE_FORMAT_NV12:
				imageSpace.lpY = imageProperties.lpY;
				imageSpace.lpUV= imageProperties.lpUV;

				imageSpace.dwLineSize[0]= dwWidth;
				imageSpace.dwLineSize[1]= dwWidth >> 1;
				break;
			case IMAGE_FORMAT_NV21:
				imageSpace.lpY = imageProperties.lpY;
				imageSpace.lpVU= imageProperties.lpVU;

				imageSpace.dwLineSize[0]= dwWidth;
				imageSpace.dwLineSize[1]= dwWidth >> 1;
				break;
			case IMAGE_FORMAT_PAL8:
				imageSpace.lpRGB = imageProperties.lpRGB;

				imageSpace.dwLineSize[0]= dwWidth;
				break;
			case IMAGE_FORMAT_RGB555:
			case IMAGE_FORMAT_RGB565:
				imageSpace.lpRGB = imageProperties.lpRGB;

				imageSpace.dwLineSize[0]= dwWidth << 1;
				break;
			case IMAGE_FORMAT_BGR24:
				imageSpace.lpRGB = imageProperties.lpRGB;

				imageSpace.dwLineSize[0]= dwWidth * 3;
				break;
			case IMAGE_FORMAT_RGB32:
			case IMAGE_FORMAT_BGR32:
				imageSpace.lpRGB = imageProperties.lpRGB;

				imageSpace.dwLineSize[0]= dwWidth << 2;
				break;
		}

		return (CDirectDraw::Draw(imageSpace,imageProperties.dwVideoType) == S_OK) ? ERROR_SUCCESS : ERROR_INTERNAL_ERROR;
	}

private:
	DWORD m_dwImageFormat;
	DWORD m_dwWidth;
	DWORD m_dwHeight;
	HWND m_hWnd;

    //CCriticalSection m_csDraw;
};


//////////////////////////////////////////////////////////////////////////

DWORD DD_Initialize(LPHANDLE lpHandle)
{
	if (lpHandle == NULL) {
		return ERROR_INVALID_PARAMETER;
	}

	*lpHandle = new CDirectDrawEx;
	return ERROR_SUCCESS;
}

DWORD DD_Uninitialize(HANDLE hHandle)
{
	CDirectDrawEx *pDirectDraw = (CDirectDrawEx *) hHandle;

	if(pDirectDraw!=NULL)
	{
		delete pDirectDraw;
		pDirectDraw = NULL;
	}
	

	return ERROR_SUCCESS;
}

DWORD DD_Draw(HANDLE hHandle, HWND hWnd, const ImageProperties *pImageProperties)
{
	if (hHandle == NULL || pImageProperties == NULL) {
		return ERROR_INVALID_PARAMETER;
	}

	CDirectDrawEx *pDirectDraw = (CDirectDrawEx *) hHandle;
	return pDirectDraw->Draw(hWnd, *pImageProperties);
}

void DD_RegisterDrawFun(HANDLE hHandle, void* DrawFun, void* user)
{
	if (hHandle == NULL)
		return;

	CDirectDrawEx *pDirectDraw = (CDirectDrawEx *) hHandle;
	pDirectDraw->RegisterDrawFun(DrawFun, user);
}

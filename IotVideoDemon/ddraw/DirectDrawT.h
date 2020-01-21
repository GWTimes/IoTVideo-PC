
#ifndef DirectDrawT_H
#define DirectDrawT_H

//////////////////////////////////////////////////////////////////////////

#include <DDraw.h>

//////////////////////////////////////////////////////////////////////////
typedef void (CALLBACK* DRAW_FUN_CALLBACK) (HDC hDC, RECT rect, void* pUser);

#pragma comment(lib, "DDraw.lib")
#pragma comment(lib, "DxGuid.lib")

//////////////////////////////////////////////////////////////////////////

struct ImageSpace
{
	enum { MAX_SPACE_COUNT = 4, };

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

	DWORD dwLineSize[MAX_SPACE_COUNT];
};

//////////////////////////////////////////////////////////////////////////

struct FormatYV12
{
	static VOID Build(DDSURFACEDESC2 &ddsd, DWORD dwWidth, DWORD dwHeight)
	{
		ZeroMemory(&ddsd, sizeof(ddsd));

		ddsd.dwSize		= sizeof(ddsd);
		ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;
		ddsd.dwFlags	= DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT;
		ddsd.dwWidth	= dwWidth;
		ddsd.dwHeight	= dwHeight;
		ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

		ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC|DDPF_YUV;
		ddsd.ddpfPixelFormat.dwYUVBitCount = 8;
		ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('Y','V','1','2');
	}
	static VOID Copy(LPBYTE lpSurface, const ImageSpace &imageSpace, DWORD dwWidth, DWORD dwHeight, LONG lPitch)
	{
		LPBYTE lpSrcY = imageSpace.lpY;
		LPBYTE lpSrcV = imageSpace.lpV;
		LPBYTE lpSrcU = imageSpace.lpU;

		DWORD dwCopyLength = 0;

		if (lpSrcY != NULL) {
			dwCopyLength = dwWidth;
			for (DWORD i=0; i<dwHeight; ++i) {
				CopyMemory(lpSurface, lpSrcY, dwCopyLength);
				lpSrcY += imageSpace.dwLineSize[0];
				lpSurface += lPitch;
			}
		}
		if (lpSrcV != NULL) {
			dwCopyLength = dwWidth >> 1;
			for (DWORD i=0; i<(dwHeight >> 1); ++i) {
				CopyMemory(lpSurface, lpSrcV, dwCopyLength);
				lpSrcV += imageSpace.dwLineSize[1];
				lpSurface += lPitch >> 1;
			}
		}
		if (lpSrcU != NULL) {
			dwCopyLength = dwWidth >> 1;
			for (DWORD i=0; i<(dwHeight >> 1); ++i) {
				CopyMemory(lpSurface, lpSrcU, dwCopyLength);
				lpSrcU += imageSpace.dwLineSize[2];
				lpSurface += lPitch >> 1;
			}
		}
	}
};

struct FormatI420
{
	static VOID Build(DDSURFACEDESC2 &ddsd, DWORD dwWidth, DWORD dwHeight)
	{
		ZeroMemory(&ddsd, sizeof(ddsd));

		ddsd.dwSize		= sizeof(ddsd);
		ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;
		ddsd.dwFlags	= DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT;
		ddsd.dwWidth	= dwWidth;
		ddsd.dwHeight	= dwHeight;
		ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

		ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC|DDPF_YUV;
		ddsd.ddpfPixelFormat.dwYUVBitCount = 8;
		ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('I','4','2','0');
	}
	static VOID Copy(LPBYTE lpSurface, const ImageSpace &imageSpace, DWORD dwWidth, DWORD dwHeight, LONG lPitch)
	{
		LPBYTE lpSrcY = imageSpace.lpY;
		LPBYTE lpSrcU = imageSpace.lpU;
		LPBYTE lpSrcV = imageSpace.lpV;

		DWORD dwCopyLength = 0;

		if (lpSrcY != NULL) {
			dwCopyLength = dwWidth;
			for (DWORD i=0; i<dwHeight; ++i) {
				CopyMemory(lpSurface, lpSrcY, dwCopyLength);
				lpSrcY += imageSpace.dwLineSize[0];
				lpSurface += lPitch;
			}
		}
		if (lpSrcU != NULL) {
			dwCopyLength = dwWidth >> 1;
			for (DWORD i=0; i<(dwHeight >> 1); ++i) {
				CopyMemory(lpSurface, lpSrcU, dwCopyLength);
				lpSrcU += imageSpace.dwLineSize[2];
				lpSurface += lPitch >> 1;
			}
		}
		if (lpSrcV != NULL) {
			dwCopyLength = dwWidth >> 1;
			for (DWORD i=0; i<(dwHeight >> 1); ++i) {
				CopyMemory(lpSurface, lpSrcV, dwCopyLength);
				lpSrcV += imageSpace.dwLineSize[1];
				lpSurface += lPitch >> 1;
			}
		}
	}
};

struct FormatYUY2
{
	static VOID Build(DDSURFACEDESC2 &ddsd, DWORD dwWidth, DWORD dwHeight)
	{
		ZeroMemory(&ddsd, sizeof(ddsd));

		ddsd.dwSize		= sizeof(ddsd);
		ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;
		ddsd.dwFlags	= DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT;
		ddsd.dwWidth	= dwWidth;
		ddsd.dwHeight	= dwHeight;
		ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

		ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC|DDPF_YUV;
		ddsd.ddpfPixelFormat.dwYUVBitCount = 16;
		ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('Y','U','Y','2');
	}
	static VOID Copy(LPBYTE lpSurface, const ImageSpace &imageSpace, DWORD dwWidth, DWORD dwHeight, LONG lPitch)
	{
		LPBYTE lpSrcYUY2 = imageSpace.lpYUY2;
		DWORD dwCopyLength = dwWidth << 1;

		for (DWORD i=0; i<dwHeight; ++i) {
			CopyMemory(lpSurface, lpSrcYUY2, dwCopyLength);
			lpSrcYUY2 += imageSpace.dwLineSize[0];
			lpSurface += lPitch;
		}
	}
};

struct FormatYVYU
{
	static VOID Build(DDSURFACEDESC2 &ddsd, DWORD dwWidth, DWORD dwHeight)
	{
		ZeroMemory(&ddsd, sizeof(ddsd));

		ddsd.dwSize		= sizeof(ddsd);
		ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;
		ddsd.dwFlags	= DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT;
		ddsd.dwWidth	= dwWidth;
		ddsd.dwHeight	= dwHeight;
		ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

		ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC|DDPF_YUV;
		ddsd.ddpfPixelFormat.dwYUVBitCount = 16;
		ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('Y','V','Y','U');
	}
	static VOID Copy(LPBYTE lpSurface, const ImageSpace &imageSpace, DWORD dwWidth, DWORD dwHeight, LONG lPitch)
	{
		LPBYTE lpSrcYVYU = imageSpace.lpYVYU;
		DWORD dwCopyLength = dwWidth << 1;

		for (DWORD i=0; i<dwHeight; ++i) {
			CopyMemory(lpSurface, lpSrcYVYU, dwCopyLength);
			lpSrcYVYU += imageSpace.dwLineSize[0];
			lpSurface += lPitch;
		}
	}
};

struct FormatUYVY
{
	static VOID Build(DDSURFACEDESC2 &ddsd, DWORD dwWidth, DWORD dwHeight)
	{
		ZeroMemory(&ddsd, sizeof(ddsd));

		ddsd.dwSize		= sizeof(ddsd);
		ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;
		ddsd.dwFlags	= DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT;
		ddsd.dwWidth	= dwWidth;
		ddsd.dwHeight	= dwHeight;
		ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

		ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC|DDPF_YUV;
		ddsd.ddpfPixelFormat.dwYUVBitCount = 16;
		ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('U','Y','V','Y');
	}
	static VOID Copy(LPBYTE lpSurface, const ImageSpace &imageSpace, DWORD dwWidth, DWORD dwHeight, LONG lPitch)
	{
		FormatYUY2::Copy(lpSurface, imageSpace, dwWidth, dwHeight, lPitch);
	}
};

struct FormatNV12
{
	static VOID Build(DDSURFACEDESC2 &ddsd, DWORD dwWidth, DWORD dwHeight)
	{
		ZeroMemory(&ddsd, sizeof(ddsd));

		ddsd.dwSize		= sizeof(ddsd);
		ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;
		ddsd.dwFlags	= DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT;
		ddsd.dwWidth	= dwWidth;
		ddsd.dwHeight	= dwHeight;
		ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

		ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC|DDPF_YUV;
		ddsd.ddpfPixelFormat.dwYUVBitCount = 8;
		ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('N','V','1','2');
	}
	static VOID Copy(LPBYTE lpSurface, const ImageSpace &imageSpace, DWORD dwWidth, DWORD dwHeight, LONG lPitch)
	{
		LPBYTE lpSrcY = imageSpace.lpY;
		LPBYTE lpSrcUV = imageSpace.lpUV;

		DWORD dwCopyLength = 0;

		if (lpSrcY != NULL) {
			dwCopyLength = dwWidth;
			for (DWORD i=0; i<dwHeight; ++i) {
				CopyMemory(lpSurface, lpSrcY, dwCopyLength);
				lpSrcY += imageSpace.dwLineSize[0];
				lpSurface += lPitch;
			}
		}
		if (lpSrcUV != NULL) {
			dwCopyLength = dwWidth;
			for (DWORD i=0; i<(dwHeight >> 1); ++i) {
				CopyMemory(lpSurface, lpSrcUV, dwCopyLength);
				lpSrcUV += imageSpace.dwLineSize[0];
				lpSurface += lPitch;
			}
		}
	}
};

struct FormatNV21
{
	static VOID Build(DDSURFACEDESC2 &ddsd, DWORD dwWidth, DWORD dwHeight)
	{
		ZeroMemory(&ddsd, sizeof(ddsd));

		ddsd.dwSize = sizeof(ddsd);
		ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;
		ddsd.dwFlags	= DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT;
		ddsd.dwWidth	= dwWidth;
		ddsd.dwHeight	= dwHeight;
		ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

		ddsd.ddpfPixelFormat.dwFlags = DDPF_FOURCC|DDPF_YUV;
		ddsd.ddpfPixelFormat.dwYUVBitCount = 8;
		ddsd.ddpfPixelFormat.dwFourCC = MAKEFOURCC('N','V','2','1');
	}
	static VOID Copy(LPBYTE lpSurface, const ImageSpace &imageSpace, DWORD dwWidth, DWORD dwHeight, LONG lPitch)
	{
		LPBYTE lpSrcY	= imageSpace.lpY;
		LPBYTE lpSrcVU	= imageSpace.lpVU;

		DWORD dwCopyLength = 0;

		if (lpSrcY != NULL) {
			dwCopyLength = dwWidth;
			for (DWORD i=0; i<dwHeight; ++i) {
				CopyMemory(lpSurface, lpSrcY, dwCopyLength);
				lpSrcY += imageSpace.dwLineSize[0];
				lpSurface += lPitch;
			}
		}
		if (lpSrcVU != NULL) {
			dwCopyLength = dwWidth;
			for (DWORD i=0; i<(dwHeight >> 1); ++i) {
				CopyMemory(lpSurface, lpSrcVU, dwCopyLength);
				lpSrcVU += imageSpace.dwLineSize[0];
				lpSurface += lPitch;
			}
		}
	}
};

//////////////////////////////////////////////////////////////////////////

struct FormatPAL8
{
	static VOID Build(DDSURFACEDESC2 &ddsd, DWORD dwWidth, DWORD dwHeight)
	{
		ZeroMemory(&ddsd, sizeof(ddsd));

		ddsd.dwSize		= sizeof(ddsd);
		ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;
		ddsd.dwFlags	= DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT;
		ddsd.dwWidth	= dwWidth;
		ddsd.dwHeight	= dwHeight;
		ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

		ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB|DDPF_PALETTEINDEXED8;
		ddsd.ddpfPixelFormat.dwRGBBitCount = 8;
	}
	static VOID Copy(LPBYTE lpSurface, const ImageSpace &imageSpace, DWORD dwWidth, DWORD dwHeight, LONG lPitch)
	{
		LPBYTE lpSrcRGB = imageSpace.lpRGB;

		DWORD dwCopyLength = dwWidth << 2;

		for (DWORD i=0; i<dwHeight; ++i) {
			CopyMemory(lpSurface, lpSrcRGB, dwCopyLength);
			lpSrcRGB += imageSpace.dwLineSize[0];
			lpSurface += lPitch;
		}
	}
};

struct FormatRGB555
{
	enum
	{
		RBitMask = 0x7C00, 
		GBitMask = 0x03E0, 
		BBitMask = 0x001F, 
	};

	static VOID Build(DDSURFACEDESC2 &ddsd, DWORD dwWidth, DWORD dwHeight)
	{
		ZeroMemory(&ddsd, sizeof(ddsd));

		ddsd.dwSize		= sizeof(ddsd);
		ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;
		ddsd.dwFlags	= DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT;
		ddsd.dwWidth	= dwWidth;
		ddsd.dwHeight	= dwHeight;
		ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

		ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
		ddsd.ddpfPixelFormat.dwRGBBitCount = 16;

		ddsd.ddpfPixelFormat.dwRBitMask = RBitMask;
		ddsd.ddpfPixelFormat.dwGBitMask = GBitMask;
		ddsd.ddpfPixelFormat.dwBBitMask = BBitMask;

		ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0;
	}
	static VOID Copy(LPBYTE lpSurface, const ImageSpace &imageSpace, DWORD dwWidth, DWORD dwHeight, LONG lPitch)
	{
		PSHORT lpSrcRGB = (PSHORT) imageSpace.lpRGB;
		PSHORT lpDstRGB = (PSHORT) lpSurface;

		for (DWORD i=0; i<dwHeight; ++i) {
			ZeroMemory(lpDstRGB, dwWidth * sizeof(SHORT));

			for (DWORD j=0; j<dwWidth; ++j) {
				lpDstRGB[j] |= (lpSrcRGB[j] & RBitMask) >> 10;
				lpDstRGB[j] |= (lpSrcRGB[j] & GBitMask);
				lpDstRGB[j] |= (lpSrcRGB[j] & BBitMask) << 10;
			}

			lpSrcRGB += imageSpace.dwLineSize[0] / sizeof(SHORT);
			lpDstRGB += lPitch / sizeof(SHORT);
		}
	}
};

struct FormatRGB565
{
	enum
	{
		RBitMask = 0xF800, 
		GBitMask = 0x07E0, 
		BBitMask = 0x001F, 
	};

	static VOID Build(DDSURFACEDESC2 &ddsd, DWORD dwWidth, DWORD dwHeight)
	{
		ZeroMemory(&ddsd, sizeof(ddsd));

		ddsd.dwSize		= sizeof(ddsd);
		ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;
		ddsd.dwFlags	= DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT;
		ddsd.dwWidth	= dwWidth;
		ddsd.dwHeight	= dwHeight;
		ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

		ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
		ddsd.ddpfPixelFormat.dwRGBBitCount = 16;

		ddsd.ddpfPixelFormat.dwRBitMask = RBitMask;
		ddsd.ddpfPixelFormat.dwGBitMask = GBitMask;
		ddsd.ddpfPixelFormat.dwBBitMask = BBitMask;

		ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0;
	}
	static VOID Copy(LPBYTE lpSurface, const ImageSpace &imageSpace, DWORD dwWidth, DWORD dwHeight, LONG lPitch)
	{
		PSHORT lpSrcRGB = (PSHORT) imageSpace.lpRGB;
		PSHORT lpDstRGB = (PSHORT) lpSurface;

		for (DWORD i=0; i<dwHeight; ++i) {
			ZeroMemory(lpDstRGB, dwWidth * sizeof(SHORT));

			for (DWORD j=0; j<dwWidth; ++j) {
				lpDstRGB[j] |= (lpSrcRGB[j] & RBitMask) >> 11;
				lpDstRGB[j] |= (lpSrcRGB[j] & GBitMask);
				lpDstRGB[j] |= (lpSrcRGB[j] & BBitMask) << 11;
			}

			lpSrcRGB += imageSpace.dwLineSize[0] / sizeof(SHORT);
			lpDstRGB += lPitch / sizeof(SHORT);
		}
	}
};

struct FormatBGR24
{
	static VOID Build(DDSURFACEDESC2 &ddsd, DWORD dwWidth, DWORD dwHeight)
	{
		ZeroMemory(&ddsd, sizeof(ddsd));

		ddsd.dwSize		= sizeof(ddsd);
		ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;
		ddsd.dwFlags	= DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT;
		ddsd.dwWidth	= dwWidth;
		ddsd.dwHeight	= dwHeight;
		ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

		ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
		ddsd.ddpfPixelFormat.dwRGBBitCount = 32;
		ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0;
	}
	static VOID Copy(LPBYTE lpSurface, const ImageSpace &imageSpace, DWORD dwWidth, DWORD dwHeight, LONG lPitch)
	{
		LPBYTE lpSrcRGB	= imageSpace.lpRGB;
		LPDWORD lpDstRGB= (LPDWORD) lpSurface;

		for (DWORD i=0; i<dwHeight; ++i) {
			ZeroMemory(lpDstRGB, dwWidth * sizeof(DWORD));

			for (DWORD j=0; j<dwWidth; ++j) {
				lpDstRGB[j] |= (lpSrcRGB[3 * j]);
				lpDstRGB[j] |= (lpSrcRGB[3 * j + 1] << 8);
				lpDstRGB[j] |= (lpSrcRGB[3 * j + 2] << 16);
			}

			lpSrcRGB += imageSpace.dwLineSize[0];
			lpDstRGB += lPitch / sizeof(DWORD);
		}
	}
};

struct FormatRGB32
{
	static VOID Build(DDSURFACEDESC2 &ddsd, DWORD dwWidth, DWORD dwHeight)
	{
		ZeroMemory(&ddsd, sizeof(ddsd));

		ddsd.dwSize		= sizeof(ddsd);
		ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;
		ddsd.dwFlags	= DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT;
		ddsd.dwWidth	= dwWidth;
		ddsd.dwHeight	= dwHeight;
		ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

		ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
		ddsd.ddpfPixelFormat.dwRGBBitCount = 32;
		ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0;
	}
	static VOID Copy(LPBYTE lpSurface, const ImageSpace &imageSpace, DWORD dwWidth, DWORD dwHeight, LONG lPitch)
	{
		LPBYTE lpSrcRGB	= imageSpace.lpRGB;
		LPDWORD lpDstRGB= (LPDWORD) lpSurface;

		for (DWORD i=0; i<dwHeight; ++i) {
			ZeroMemory(lpDstRGB, dwWidth * sizeof(DWORD));

			for (DWORD j=0; j<dwWidth; ++j) {
				lpDstRGB[j] |= (lpSrcRGB[4 * j]);
				lpDstRGB[j] |= (lpSrcRGB[4 * j + 1] << 8);
				lpDstRGB[j] |= (lpSrcRGB[4 * j + 2] << 16);
				lpDstRGB[j] |= (lpSrcRGB[4 * j + 3] << 24);
			}

			lpSrcRGB += imageSpace.dwLineSize[0];
			lpDstRGB += lPitch / sizeof(DWORD);
		}
	}
};

struct FormatBGR32
{
	static VOID Build(DDSURFACEDESC2 &ddsd, DWORD dwWidth, DWORD dwHeight)
	{
		ZeroMemory(&ddsd, sizeof(ddsd));

		ddsd.dwSize		= sizeof(ddsd);
		ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;
		ddsd.dwFlags	= DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH|DDSD_PIXELFORMAT;
		ddsd.dwWidth	= dwWidth;
		ddsd.dwHeight	= dwHeight;
		ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

		ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
		ddsd.ddpfPixelFormat.dwRGBBitCount = 32;
		ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0;
	}
	static VOID Copy(LPBYTE lpSurface, const ImageSpace &imageSpace, DWORD dwWidth, DWORD dwHeight, LONG lPitch)
	{
		LPBYTE lpSrcRGB	= imageSpace.lpRGB;
		LPDWORD lpDstRGB= (LPDWORD) lpSurface;

		for (DWORD i=0; i<dwHeight; ++i) {
			ZeroMemory(lpDstRGB, dwWidth * sizeof(DWORD));

			for (DWORD j=0; j<dwWidth; ++j) {
				lpDstRGB[j] |= (lpSrcRGB[4 * j] << 16);
				lpDstRGB[j] |= (lpSrcRGB[4 * j + 1] << 8);
				lpDstRGB[j] |= (lpSrcRGB[4 * j + 2]);
				lpDstRGB[j] |= (lpSrcRGB[4 * j + 3] << 24);
			}

			lpSrcRGB += imageSpace.dwLineSize[0];
			lpDstRGB += lPitch / sizeof(DWORD);
		}
	}
};

//////////////////////////////////////////////////////////////////////////

template <DWORD dwUnique>
class DirectDrawT
{
	typedef VOID (*CopyCallback)(LPBYTE lpSurface, const ImageSpace &imageSpace, DWORD dwWidth, DWORD dwHeight, LONG lPitch);

public:
	DirectDrawT()
		:	m_hWnd(NULL), 
		m_dwWidth(0), 
		m_dwHeight(0), 
		m_pClipper(NULL), 
		m_pDirectDraw(NULL), 
		m_pSurfacePrimary(NULL), 
		m_pSurfaceOffScreen(NULL), 
//		m_pSurfaceOSD(NULL), 
		m_copyCallback(NULL),
		m_DrawCallback(NULL)
	{
	}
	~DirectDrawT()
	{
		Release();
	}

	HRESULT Draw(const ImageSpace &imageSpace,const DWORD &dwVideoType)
	{
		HRESULT hr = E_INVALIDARG;

		RECT rectSrc = { 0 };
		RECT rectDst = { 0 };
		RECT rectWnd = { 0 };

		DDSURFACEDESC2 ddOffScreenSurfaceDesc = { 0 };
		if (m_copyCallback == NULL || m_pSurfaceOffScreen == NULL || m_dwWidth == 0 || m_dwHeight == 0) {
			goto Exit;
		}

		ddOffScreenSurfaceDesc.dwSize = sizeof(ddOffScreenSurfaceDesc);
		hr = m_pSurfaceOffScreen->Lock(NULL, &ddOffScreenSurfaceDesc, DDLOCK_WAIT|DDLOCK_WRITEONLY, NULL);
		if (hr != S_OK) {
			//屏保后进来，错误码居然不是DDERR_SURFACELOST
			m_pSurfaceOffScreen->Restore();
			m_pSurfacePrimary->Restore();

			goto Exit;
		}

		m_copyCallback((LPBYTE) ddOffScreenSurfaceDesc.lpSurface, imageSpace, ddOffScreenSurfaceDesc.dwWidth, ddOffScreenSurfaceDesc.dwHeight, ddOffScreenSurfaceDesc.lPitch);
		m_pSurfaceOffScreen->Unlock(NULL);

		//解码时，每次取16*16解码，不足时用最后一位代替。以至于导致视频下方有时候显示不正常，因此长宽各减16
		/*if( (ddOffScreenSurfaceDesc.dwWidth%16==0)&&(ddOffScreenSurfaceDesc.dwHeight%16==0))
		{
			rectSrc.right	= ddOffScreenSurfaceDesc.dwWidth;
			rectSrc.bottom	= ddOffScreenSurfaceDesc.dwHeight;
		}
		else*/
		{
			rectSrc.right	= ddOffScreenSurfaceDesc.dwWidth-16;
			rectSrc.bottom	= ddOffScreenSurfaceDesc.dwHeight-16;
		}
		if(5 == dwVideoType && ddOffScreenSurfaceDesc.dwWidth%64==0) //若是H265裁掉 32位
		{
			rectSrc.right = rectSrc.right -32;
		}
//		ATLTRACE(_T("\n width = %d, height = %d"), ddOffScreenSurfaceDesc.dwWidth, ddOffScreenSurfaceDesc.dwHeight);
		::GetClientRect(m_hWnd, &rectDst);
		::GetClientRect(m_hWnd, &rectWnd);
		::ClientToScreen(m_hWnd, (LPPOINT) &(rectDst.left));
		::ClientToScreen(m_hWnd, (LPPOINT) &(rectDst.right));

// 		if (m_rectWnd.right != rectWnd.right || m_rectWnd.bottom != rectWnd.bottom || m_pSurfaceOSD == NULL)
// 		{
// 			if (m_pSurfaceOSD != NULL)
// 			{
// 				m_pSurfaceOSD->SetClipper(NULL);
// 				m_pSurfaceOSD->Release();
// 				m_pSurfaceOSD = NULL;
// 			}
// 
// 			DDSURFACEDESC2	ddsd;
// 			ZeroMemory(&ddsd, sizeof(ddsd));
// 			ddsd.dwSize			= sizeof(ddsd);
// 			ddsd.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT ;
// 			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
// 			ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
// 			ddsd.dwWidth        = rectDst.right - rectDst.left ;
// 			ddsd.dwHeight       = rectDst.bottom - rectDst.top;
// 
// 			if( FAILED(m_pDirectDraw->CreateSurface(&ddsd, &m_pSurfaceOSD, NULL)) )
// 			{
// 				ATLTRACE("Create BltBuffer Failed\n");
// 				goto Exit;
// 			}
// 			m_rectWnd = rectWnd;
// 		}

// 		while (TRUE)
// 		{
// 			hr = m_pSurfaceOSD->Blt(NULL, m_pSurfaceOffScreen, &rectSrc, DDBLT_WAIT, NULL);
// 			if (hr == DDERR_SURFACELOST) 
// 			{
// 				m_pSurfaceOSD->Restore();
// 				m_pSurfaceOffScreen->Restore();
// 			}
// 
// 			if (hr != DDERR_WASSTILLDRAWING) 
// 			{
// 				break;
// 			}
// 		}
// 
// 
// 		HDC hDC = NULL;
// 		HRESULT ddrval = m_pSurfaceOSD->GetDC(&hDC);
// 		if (ddrval == DD_OK && hDC != NULL)
// 		{
// 			if (m_DrawCallback != NULL)
// 			{
// 				m_DrawCallback(hDC, rectWnd, m_user);
// 			}
// 			m_pSurfaceOSD->ReleaseDC(hDC);
// 		}

		while (TRUE)
		{
//			hr = m_pSurfacePrimary->Blt(&rectDst, m_pSurfaceOSD, &rectWnd, DDBLT_WAIT, NULL);
			hr = m_pSurfacePrimary->Blt(&rectDst, m_pSurfaceOffScreen, &rectSrc, DDBLT_WAIT, NULL);
			if (hr == DDERR_SURFACELOST) 
			{
//				m_pSurfaceOSD->Restore();
				m_pSurfaceOffScreen->Restore();
				m_pSurfacePrimary->Restore();//离屏不显示问题修改 add by lingyz
			}

			if (hr != DDERR_WASSTILLDRAWING) 
			{
				break;
			}

		}

		hr = S_OK;
Exit:
		return hr;
	}

	template <typename PixelFormatT>
	HRESULT Create(HWND hWnd, DDSURFACEDESC2 &ddOffScreenSurfaceDesc)
	{
		Release();

		HRESULT hr = DirectDrawCreateEx(NULL, (LPVOID *) &m_pDirectDraw, IID_IDirectDraw7, NULL);
		if (hr != S_OK) {
			goto Exit;
		}

		hr = m_pDirectDraw->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
		if (hr != S_OK) {
			goto Exit;
		}

		{
			DDSURFACEDESC2 ddPrimarySurfaceDesc = { 0 };
			ddPrimarySurfaceDesc.dwSize			= sizeof(ddPrimarySurfaceDesc);
			ddPrimarySurfaceDesc.dwFlags		= DDSD_CAPS;
			ddPrimarySurfaceDesc.ddsCaps.dwCaps	= DDSCAPS_PRIMARYSURFACE;
			hr = m_pDirectDraw->CreateSurface(&ddPrimarySurfaceDesc, &m_pSurfacePrimary, NULL);
			if (hr != S_OK) {
				goto Exit;
			}
			hr = m_pDirectDraw->CreateClipper(0, &m_pClipper, NULL);
			if (hr != S_OK) {
				goto Exit;
			}
			hr = m_pClipper->SetHWnd(0, hWnd);
			if (hr != S_OK) {
				goto Exit;
			}
			m_pSurfacePrimary->SetClipper(m_pClipper);
		}

		hr = m_pDirectDraw->CreateSurface(&ddOffScreenSurfaceDesc, &m_pSurfaceOffScreen, NULL);
		if (hr != S_OK) {
			goto Exit;
		}

		m_hWnd			= hWnd;
		m_dwWidth		= ddOffScreenSurfaceDesc.dwWidth;
		m_dwHeight		= ddOffScreenSurfaceDesc.dwHeight;
		m_copyCallback	= PixelFormatT::Copy;

Exit:
		if (hr != S_OK) {
			Release();
		}
		return hr;
	}

	VOID Release()
	{
		m_hWnd			= NULL;
		m_dwWidth		= 0;
		m_dwHeight		= 0;
		m_copyCallback	= NULL;

		if (m_pClipper != NULL) {
			m_pClipper->Release();
			m_pClipper = NULL;
		}
		if (m_pSurfacePrimary != NULL) {
			m_pSurfacePrimary->SetClipper(NULL);
			m_pSurfacePrimary->Release();
			m_pSurfacePrimary = NULL;
		}
		if (m_pSurfaceOffScreen != NULL) {
			m_pSurfaceOffScreen->SetClipper(NULL);
			m_pSurfaceOffScreen->Release();
			m_pSurfaceOffScreen = NULL;
		}
// 		if (m_pSurfaceOSD != NULL) {
// 			m_pSurfaceOSD->SetClipper(NULL);
// 			m_pSurfaceOSD->Release();
// 			m_pSurfaceOSD = NULL;
// 		}

		if (m_pDirectDraw != NULL) {
			m_pDirectDraw->Release();
			m_pDirectDraw = NULL;
		}
	}
	void RegisterDrawFun(void* DrawFun, void* user)
	{
		m_DrawCallback = (DRAW_FUN_CALLBACK)DrawFun;
		m_user = user;
	}
private:
	LPDIRECTDRAW7 m_pDirectDraw;
	LPDIRECTDRAWCLIPPER m_pClipper;
	LPDIRECTDRAWSURFACE7 m_pSurfacePrimary;
	LPDIRECTDRAWSURFACE7 m_pSurfaceOffScreen;
//	LPDIRECTDRAWSURFACE7 m_pSurfaceOSD;

	HWND m_hWnd;
	RECT m_rectWnd;
	DWORD m_dwWidth;
	DWORD m_dwHeight;

	CopyCallback m_copyCallback;

	DRAW_FUN_CALLBACK volatile m_DrawCallback;
	void* m_user;
};

//////////////////////////////////////////////////////////////////////////

typedef DirectDrawT<0> CDirectDraw;

//////////////////////////////////////////////////////////////////////////

#endif


//	--------------------------------------------------------------
//	
//	Copyright (C) 2009 - All Rights Reserved.
//	
//	Author:		LiuYin
//	File:		CriticalSection
//	Version: 	1.0
//	Date: 		2009-8-10
//	
//	Purpose:	
//	
//	--------------------------------------------------------------

#ifndef CriticalSection_H
#define CriticalSection_H

//////////////////////////////////////////////////////////////////////////

#include <Windows.h>

//////////////////////////////////////////////////////////////////////////

template <DWORD dwUnique>
class CriticalSectionT
{
public:
	CriticalSectionT() throw()
	{
		ZeroMemory(&m_criticalSection, sizeof(m_criticalSection));

		Init();
	}
	~CriticalSectionT()
	{
		Term();
	}

	BOOL Lock() throw()
	{
		EnterCriticalSection(&m_criticalSection);
		return TRUE;
	}
	BOOL Unlock() throw()
	{
		LeaveCriticalSection(&m_criticalSection);
		return TRUE;
	}

private:
	BOOL Init() throw()
	{
		HRESULT hRes = E_FAIL;
		__try {
			InitializeCriticalSection(&m_criticalSection);
			hRes = S_OK;
		}
		// structured exception may be raised in low memory situations
		__except(STATUS_NO_MEMORY == GetExceptionCode()) {
			hRes = E_OUTOFMEMORY;
		}
		return SUCCEEDED(hRes);
	}
	BOOL Term() throw()
	{
		DeleteCriticalSection(&m_criticalSection);
		return TRUE;
	}

private:
	CRITICAL_SECTION m_criticalSection;
};

//////////////////////////////////////////////////////////////////////////

typedef CriticalSectionT<0> CCriticalSectionX;

//////////////////////////////////////////////////////////////////////////

#endif

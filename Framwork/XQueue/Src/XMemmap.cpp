/*
Module : XMEMMAP.CPP
Purpose: Implementation for an MFC class to wrap memory mapped files
Created: PJN / MEMMAP/1 / 19-03-1997
History: PJN / 31-3-1998  1) Class now avoids trying to lock the mutex if only read access is required
                          2) User now has the option of specifying whether a file should be mapped with 
                             A Null terminator at the end. Can prove helpful when you want to use some
                             of the "C" runtime functions on the pointer returned.
         PJN / 20-4-1998  1) Now uses GetFileSize SDK call instead of GetFileInformationByHandle as a
                             more "reliable" way to determine file length.
                          2) Included TRACE statements to call GetLastError in all places where 
                             SDK functions fail
         PJN / 29-5-1998  1) Mapping a file now has the option of making it named or not.
         PJN / 22-10-1998 1) Fixed a bug in a number of calls to CreateMappingName when the classes were 
                             being used to share memory.
                          2) Tidy up of the demo app including:
                             a) Made the amount of text being shared a constant of MAX_EDIT_TEXT instead
                             of hardcoding it to 20 everywhere in the sample.
                             b) Changed where the timer is being created to OnInitDialog
                             c) Tidied up the initialisation sequence in OnInitDialog
                             d) Now using _tcscpy instead of _tcsncpy to ensure array is null terminated
                             e) Fixed resource.h which was causing the resources to fail to compile
                             f) Removed unnecessary symbols from resource.h
                             g) Optimized the way the OnTimer code works to only update the text when it
                             has changed in the MMF. This means that you can type continuously into the
                             edit control.
                          3) New documentation in the form of a HTML file.
                          4) Sample now ships as standard with VC 5 workspace files
         PJN / 30-3-1999  1) Code is now unicode compliant
                          2) Code now supports growable MMF's
                          3) Addition of accessor functions for file handle and file mapping handle
         PJN / 21-4-1999  1) Works around a Window bug where you try to memory map a zero length file on
                             Windows 95 or 98.
         PJN / 24-4-2000  1) Fixed a simple typo problem in a TRACE statement when compiled for UNICODE
         PJN / 07-3-2001  1) Updated copyright information   
                          2) Fixed problem where mutex was not being locked when read only access to 
                          the memory mapped file was desired. Access to the MMF should be synchronised
                          irrespective of the access mode.
         PJN / 02-4-2001  1) Now supports passing in a security descriptor to MapFile and MapMemory
                          2) Now supports opening a specified portion of a file, rather than always mapping
                          all of the file
         PJN / 05-10-2003 1) Updated copyright details.
                          2) Fixed a small typo in the description of the FSCTL_SET_SPARSE IOCTL. Thanks to 
                          amores perros for reporting this.
                          3) Fixed a minor tab indentation problem at the start of the MapFile method. Again
                          thanks to amores perros for reporting this.
                          4) Removed the unnecessary AssertValid function. Again thanks to amores perros for 
                          reporting this.
         PJN / 17-11-2003 1) Fixed a memory leak in UnMap as reported by Bart Duijndam using 
                          "Memory Validator". The memory leak may in fact not be real, but the code change 
                          avoids the reported problem.

Modified:  Hans Dietrich 16 February 2004
           - Removed dependency on MFC

Copyright (c) 1997 - 2003 by PJ Naughter.  (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/

#include <windows.h>
#include <stdio.h>
#include <crtdbg.h>
#include <tchar.h>
#define TRACE __noop

#include "xmemmap.h"

#pragma warning(disable : 4127 4312 4267)		// conditional expression is constant
									// (needed for _ASSERTE)


#ifndef _WINIOCTL_
#pragma warning(disable : 4201)		// nonstandard extension used : nameless struct/union
#include <winioctl.h>
#endif


//Define taken from the Platform SDK, means that CXMemMapFile will compile 
//correctly even on systems which do not have the Platform SDK installed
#ifndef FSCTL_SET_SPARSE
#define FSCTL_SET_SPARSE                CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 49, METHOD_BUFFERED, FILE_WRITE_DATA)
#endif



///////////////////////////////// Implementation //////////////////////////////

CXMemMapFile::CXMemMapFile()
{
  //Initialise variables to sane values
  m_hFile = INVALID_HANDLE_VALUE;
  m_hMapping = NULL;
  m_bReadOnly = TRUE;
  m_bAppendNull = FALSE;
  m_lpData = NULL;
  m_bOpen = FALSE;
  //m_hMutex = NULL;
  m_dwLength = 0;
  m_szMappingName[0] = _T('\0');
}

CXMemMapFile::~CXMemMapFile()
{
  UnMap();
}

BOOL CXMemMapFile::MapFile(LPCTSTR lpszFilename, 
						   BOOL bReadOnly, 
						   DWORD dwShareMode, 
						   BOOL bAppendNull, 
						   BOOL bNamed, 
						   BOOL bGrowable, 
						   DWORD dwStartOffset, 
						   DWORD dwNumberOfBytesToMap, 
						   LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	//Work out the file access flags
	m_bReadOnly = bReadOnly;
	DWORD dwDesiredFileAccess = GENERIC_READ;
	if (!m_bReadOnly)
		dwDesiredFileAccess |= GENERIC_WRITE;

	//store away the append Null flag
	m_bAppendNull = bAppendNull;

	//Open the real file on the file system
	m_hFile = CreateFile(lpszFilename, dwDesiredFileAccess, dwShareMode, NULL, 
	OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		TRACE(_T("Failed in call to CreateFile, GetLastError returned %d\n"), 
			GetLastError());
		UnMap();
		return FALSE;
	}

	// Make the file sparse, if requested to make the memory mapped file growable
	if (bGrowable)
	{
		DWORD dw;
		if (!::DeviceIoControl(m_hFile, FSCTL_SET_SPARSE, NULL, 0, NULL, 0, &dw, NULL))
		{
			TRACE(_T("Failed in call to make file sparse, You need Windows 2000 ")
				  _T("and an NTFS 5 volume for this !!, GetLastError returned %d\n"), 
				  GetLastError());
			UnMap();
			return FALSE;
		}
	}

	// Get the size of the file we are mapping
	DWORD dwFileSizeHigh=0;
	DWORD dwLength = GetFileSize(m_hFile, &dwFileSizeHigh);
	if (dwLength == 0xFFFFFFFF)
	{
		//There was an error calling GetFileSize
		TRACE(_T("Failed in call to GetFileSize, GetLastError returned %d\n"), 
			GetLastError());
		UnMap();
		return FALSE;
	}

	//Fail if file is greater than 4GB in size
	if (dwFileSizeHigh)
	{
		//There was an error calling GetFileSize
		TRACE(_T("File size is greater than 4GB, Memory mapping this file ")
			  _T("size will not work until CXMemMapFile supports Win64 !!\n"));
		UnMap();
		return FALSE;
	}

	//Fail if file is 0 length in size, calling CreateFileMapping on a 
	//zero length file on 95/98 can cause problems
	if (dwFileSizeHigh == 0 && dwLength == 0)
	{
		TRACE(_T("File size is 0, not attempting to memory map the file\n"));
		UnMap();
		return FALSE;
	}

	//Now work out the size of the mapping we will be performing
	if (dwNumberOfBytesToMap)
		m_dwLength = dwNumberOfBytesToMap;
	else
		m_dwLength = dwLength;

	//Do the actual mapping
	CreateMappingName(lpszFilename, 
		m_szMappingName, sizeof(m_szMappingName)/sizeof(TCHAR), bNamed);

	return MapHandle(m_hFile, lpSecurityAttributes, dwStartOffset);
}

BOOL CXMemMapFile::MapMemory(LPCTSTR lpszName, 
							 DWORD dwBytes, 
							 BOOL bReadOnly, 
							 LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	m_dwLength = dwBytes;
	m_bReadOnly = bReadOnly;
	m_bAppendNull = FALSE;
	CreateMappingName(lpszName, 
		m_szMappingName, sizeof(m_szMappingName)/sizeof(TCHAR), TRUE);
	TRACE(_T("m_szMappingName=%s\n"), m_szMappingName);
	return MapHandle((HANDLE)0xFFFFFFFF, lpSecurityAttributes, 0);
}

BOOL CXMemMapFile::MapExistingMemory(LPCTSTR lpszName, DWORD dwBytes, BOOL bReadOnly)
{
	m_dwLength = dwBytes;
	m_bReadOnly = bReadOnly;

	DWORD dwDesiredAccess = (!bReadOnly) ? FILE_MAP_WRITE : FILE_MAP_READ;
	CreateMappingName(lpszName, 
		m_szMappingName, sizeof(m_szMappingName)/sizeof(TCHAR), TRUE);
	m_hMapping = OpenFileMapping(dwDesiredAccess, 0, m_szMappingName);
	if (m_hMapping == NULL)
	{
		TRACE(_T("Failed in call to OpenFileMapping, GetLastError returned %d\n"), 
			GetLastError());
		UnMap();
		return FALSE;
	}

	//Map the view
	m_lpData = MapViewOfFile(m_hMapping, dwDesiredAccess, 0, 0, m_dwLength);

	//Create the mutex to sync access
	//m_hMutex = CreateMutex(NULL, FALSE, CreateMutexName());

	BOOL bRet = m_lpData != NULL;

	if (bRet)
		m_bOpen = TRUE;

	return bRet;
}

BOOL CXMemMapFile::MapHandle(HANDLE hHandle, 
							 LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
							 DWORD dwStartOffset)
{
	//Create the file mapping object
	DWORD flProtect = (!m_bReadOnly) ? PAGE_READWRITE : PAGE_READONLY;

	//work out the length of the file mapping to create
	DWORD dwLength = m_dwLength;
	if (m_bAppendNull)
		dwLength += 2;

	m_hMapping = ::CreateFileMapping(hHandle, lpSecurityAttributes, flProtect, 0, 
						dwLength, m_szMappingName);
	if (m_hMapping == NULL)
	{
		TRACE(_T("Failed in call to CreateFileMapping, GetLastError returned %d\n"), 
			GetLastError());
		UnMap();
		return FALSE;
	}

	//Map the view
	DWORD dwDesiredAccess = (!m_bReadOnly) ? FILE_MAP_WRITE : FILE_MAP_READ;
	m_lpData = MapViewOfFile(m_hMapping, dwDesiredAccess, 0, dwStartOffset, dwLength);

	//null terminare if asked to do so
	if (m_bAppendNull && m_lpData)
	{
		//use two Nulls just incase the data as viewed as an array of wide characters
		BYTE* lpData = (BYTE*) m_lpData;
		lpData[m_dwLength] = 0;
		lpData[m_dwLength+1] = 0;
	}

	//Create the mutex to sync access
	//m_hMutex = CreateMutex(lpSecurityAttributes, FALSE, CreateMutexName());

	BOOL bRet = m_lpData != NULL;

	if (bRet)
		m_bOpen = TRUE;

	return bRet;
}

#if 0  // -----------------------------------------------------------
LPVOID CXMemMapFile::Open(DWORD dwTimeout)
{
	if (m_lpData == NULL)
		return NULL;

	//Synchronise access to the MMF using the named mutex
	DWORD dwResult = WaitForSingleObject(m_hMutex, dwTimeout);

	if (dwResult == WAIT_OBJECT_0)
	{
		m_bOpen = TRUE;
		return m_lpData;
	}

	return NULL;
}
#endif // -----------------------------------------------------------

BOOL CXMemMapFile::Close()
{
	//Release our interest in this MMF
	if (!m_bOpen)
		return FALSE;

	m_bOpen = FALSE;
	//ReleaseMutex(m_hMutex);

	return TRUE;
}

BOOL CXMemMapFile::Flush()
{
	if (m_lpData == NULL)
		return FALSE;

	return FlushViewOfFile(m_lpData, 0);
}

#if 0  // -----------------------------------------------------------
CString CXMemMapFile::CreateMutexName() const
{
	return m_sMappingName + _T("MUTEX");
}
#endif // -----------------------------------------------------------

void CXMemMapFile::UnMap()
{
	//Close any views which may be open
	Close();

	//unmap the view
	if (m_lpData != NULL)
	{
		FlushViewOfFile(m_lpData, 0);
		UnmapViewOfFile(m_lpData);
		m_lpData = NULL;
	}

	//remove the file mapping
	if (m_hMapping != NULL)
	{
		CloseHandle(m_hMapping);
		m_hMapping = NULL;
	}

	//close the file system file if its open
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}

	//Close the mutex we have been using
#if 0  // -----------------------------------------------------------
	if (m_hMutex != NULL)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}
#endif // -----------------------------------------------------------

	//Reset the remaining member variables
	m_bReadOnly = TRUE;
	m_szMappingName[0] = _T('\0');
	m_dwLength = 0;
}

DWORD CXMemMapFile::CreateMappingName(LPCTSTR lpszName, 
									  LPTSTR lpszMappingName, 
									  DWORD dwSize, 
									  BOOL bNamed)
{
	TRACE(_T("in CXMemMapFile::CreateMappingName\n"));

	_ASSERTE(lpszName && (lpszName[0] != _T('\0')));
	if (!lpszName || (lpszName[0] == _T('\0')))
		return 0;

	_ASSERTE(lpszMappingName);
	_ASSERTE(dwSize > 2);
	if (!lpszMappingName || (dwSize <= 2))
		return 0;

	ZeroMemory(lpszMappingName, dwSize*sizeof(TCHAR));

	if (bNamed)
	{
		_tcsncpy(lpszMappingName, lpszName, dwSize-1);

		// Replace all '\' by '_'
		for (size_t i = 0; i < _tcslen(lpszName); i++)
		{
			if (lpszMappingName[i] == _T('\\'))
				lpszMappingName[i] = _T('_');
		}
		_tcsupr(lpszMappingName);
	}

	return _tcslen(lpszMappingName);
}

DWORD CXMemMapFile::GetMappingName(LPTSTR lpszName, DWORD dwSize) const
{
	_ASSERTE(lpszName);
	_ASSERTE(dwSize > 2);
	if (!lpszName || (dwSize <= 2))
		return 0;

	ZeroMemory(lpszName, dwSize*sizeof(TCHAR));
	_tcsncpy(lpszName, m_szMappingName, dwSize-1);

	return _tcslen(lpszName);
}

void CXMemMapFile::Dump() const
{
	TRACE(_T("CXMemMapFile dump:\n"));
	TRACE(_T("    m_hFile=0x%08x\n"),     m_hFile);
	TRACE(_T("    m_hMapping=0x%08x\n"),  m_hMapping);
	TRACE(_T("    m_lpData=0x%08x\n"),    m_lpData);
	TRACE(_T("    m_szMappingName=%s\n"), m_szMappingName);
	TRACE(_T("    m_bOpen=%d\n"),         m_bOpen);
	TRACE(_T("    m_dwLength=%d\n"),      m_dwLength);
}

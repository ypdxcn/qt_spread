/*
Module : XMEMMAP.H
Purpose: Interface for an MFC class to wrap memory mapped files
Created: PJN / 30-07-1997

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


////////////////////////////////// Macros ///////////////////////////

#ifndef XMEMMAP_H
#define XMEMMAP_H

#include "XQueueDll.h"

/////////////////////////// Classes /////////////////////////////////

class XQUEUE_CLASS CXMemMapFile
{
public:
//Constructors / Destructors
	CXMemMapFile();
	~CXMemMapFile();

	//Methods
	BOOL	MapFile(LPCTSTR lpszFilename, 
					BOOL bReadOnly = FALSE, 
					DWORD dwShareMode = 0, 
					BOOL bAppendNull = FALSE, 
					BOOL bNamed = FALSE, 
					BOOL bGrowable = FALSE,
					DWORD dwStartOffset=0, 
					DWORD dwNumberOfBytesToMap=0,
					LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
	BOOL	MapMemory(LPCTSTR lpszName, 
					  DWORD dwBytes, 
					  BOOL bReadOnly = FALSE,
					  LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
	BOOL	MapExistingMemory(LPCTSTR lpszName, DWORD dwBytes = 0, BOOL bReadOnly = FALSE);
	void	UnMap();
	//LPVOID	Open(/*DWORD dwTimeout = INFINITE*/);
	BOOL	Close();
	BOOL	Flush();

	//Accessors
	DWORD	GetMappingName(LPTSTR lpszName, DWORD dwSize) const;
	HANDLE	GetFileHandle() const { return m_hFile; };
	HANDLE	GetFileMappingHandle() const { return m_hMapping; };
	BOOL	IsOpen() const { return m_bOpen; };
	DWORD	GetLength() const { return m_dwLength; };
	LPVOID	GetMappedAddress() const { return m_lpData; }

	//Diagnostics / Debug
	void	Dump() const;

protected:
	BOOL	MapHandle(HANDLE hHandle, 
					  LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
					  DWORD dwStartOffset);
	DWORD	CreateMappingName(LPCTSTR lpszName, 
							  LPTSTR lpszMappingName, 
							  DWORD dwSize, 
							  BOOL bNamed);
	//CString CreateMutexName() const;

	HANDLE	m_hFile;
	HANDLE	m_hMapping;
	BOOL	m_bReadOnly;
	BOOL	m_bAppendNull;
	LPVOID	m_lpData;
	TCHAR	m_szMappingName[_MAX_PATH*2];
	BOOL	m_bOpen;
	//HANDLE  m_hMutex;
	DWORD	m_dwLength;
};

#endif

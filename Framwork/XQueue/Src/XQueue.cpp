// XQueue.cpp  Version 1.2
//
// Author: Hans Dietrich
//         hdietrich@gmail.com
//
// Description:
//     XQueue.cpp implements CXQueue, a class to create and manage an
//     interprocess queue.  CXQueue is intended to offload operations
//     from client app to server app - for example, performing logging
//     actions;  or server might function as network gateway, so
//     that details would be transparent to client.  CXQueue supports
//     one-way FIFO (first-in first-out) communication between multiple writers
//     (clients) and one reader (server) that are running on same machine.
//     Communication is one-way only - from client to server.  CXQueue offers
//     no delivery guarantee.
//
//     CXQueue may be used in either MFC or SDK programs.  It has been tested
//     on Windows 98, Windows 2000, and Windows XP.
//
// Background:
//     Although Windows NT-based platforms provide mechanisms such as
//     mailslots and named pipes that can be used to emulate queue, these
//     are not available on Win9x systems.  There have been some superficial
//     implementations of queues using shared memory and other techniques,
//     but these have been limited in one of two ways:  1) they are limited
//     to only intraprocess, not interprocess communication;  or 2) they are
//     synchronous, meaning that queue is limited to one entry, and the
//     server must remove and process entry before client can add
//     another.
//
// An Alternative:
//     If you are reading this, you are probably also familiar with the
//     Microsoft queue solution:  MSMQ.  This is an extremely powerful tool
//     that is mandatory in some situations.  However, it is also very complex,
//     non-trivial to install and set up, and has some issues on Win9x systems.
//
//     For more information on MSMQ, please refer to Microsoft site at
//         http://www.microsoft.com/windows2000/technologies/communications/msmq
//
//     For information concerning issues with MSMQ, please refer to the
//     Usenet news groups:
//         microsoft.public.msmq.programming
//         microsoft.public.msmq.setup
//         microsoft.public.msmq.deployment
//         microsoft.public.msmq.networking
//
// Concept & Facilities:
//     The CXQueue class incorporates both client and server API
//     set.  The server calls CXQueue::Create(), while client(s) call
//     CXQueue::Open().
//
//     CXQueue is based on shared memory using memory-mapped files.  Access
//     to shared memory is controlled by means of two kernel objects: an
//     event (used by clients to ensure quiescence, to allow server to
//     free some space); and mutex, to control actual writing to queue.
//
//     The shared memory (hereafter called MMF, for memory-mapped file),
//     is composed of fixed-size blocks.  The fixed size ensures that there
//     will be no fragmentation, and eliminates need for garbage
//     collection.  The first block of MMF is used for CXQueue variables.
//     Beginning with second block, all further blocks may be used for
//     data blocks.
//
//     The first (variable) block holds data that is used to manage and monitor
//     queue.  The data blocks are managed by use of two doubly-linked
//     lists:  free list and used list.  Initially, all blocks are
//     assigned to free list.  When client writes to queue, one or
//     more blocks are allocated from free list and transferred to the
//     used list.  This involves simply relinking block numbers; no
//     physical block copying is done, so it is very fast.
//
//     The client may write any number of bytes to queue.  The default
//     queue block size is 1024 bytes, but this may be easily changed by
//     modifying one line of code.  Typically client-->server message is
//     much less than 1024 bytes - usually, it is between 10 and 200 bytes.
//     To assist in determining optimal block size, CXQueue monitors
//     and stores maximum size of client write in queue variable
//     block.  You can then use this information to adjust size of
//     queue blocks.
//
//     Regardless of block size chosen, it should be expected that there
//     will be client writes that exceed block size.  When this happens,
//     CXQueue determines how many blocks will be needed, and writes data
//     to blocks, splitting data as necessary.  (Note:  data blocks
//     each have header with back and forward links and other information,
//     so there is less than 1024 bytes available in each block for data).
//     If multiple blocks are necessary, continuation flag is set in the
//     block header, to indicate that there is another block (which can be
//     found by means of forward link).
//
//     The links that have been mentioned are block numbers.  Data blocks
//     are numbered from 0 to N-1, although first data block is actually
//     second block in MMF.  To calculate byte address for any
//     block, formula is:
//         block_address = (block_no * BLOCK_SIZE) + base_address
//     where
//         block_no is zero-based number of data block,
//         BLOCK_SIZE is fixed number of bytes in a block,
//         and base_address is byte address of first data block,
//         which is simply address returned by MapViewOfFile(), plus
//         BLOCK_SIZE (to account for variable block).
//
//     Note that there is no guarantee that blocks will be contiguous
//     in multi-block client write.
//
//     When client has added an entry to queue, notification event
//     informs server that queue needs to be serviced.  The server
//     then performs read on queue (usually, two reads, with first
//     read returning only size of queue entry).  Then server
//     reads data and returns blocks to free list.  Because
//     only server manipulates blocks already in used list, it
//     is not necessary to lock queue (using mutex) until server
//     actually frees blocks.  This optimization helps to reduce the
//     time that clients are prevented from writing.
//
//     As mentioned in previous paragraph, mutex is used to control
//     write access to queue.  An event object that was mentioned
//     earlier is also used to synchronize queue access.  This event
//     object is used only by clients.  Here is why:  when client wants
//     to write, first thing it must do is determine if there are
//     enough free blocks to accommodate entire write.  If there are,
//     it can then write.  But if there are not, it would do no good to use
//     mutex at this point, because using mutex would lock out
//     server as well as all clients, so server would not be able
//     to free any blocks.  The client would then be waiting for server
//     to free some blocks, and server would be waiting for client
//     to release mutex.
//
//     What event object does is to prevent clients from starting any
//     new writes.  Since no one will be adding anything to queue, the
//     server will have chance to free some blocks.  When enough blocks
//     become free, waiting client can complete write, and then
//     set event to allow other clients to write.
//
//     To ensure that messages are processed in first-in first-out manner,
//     newly-written blocks are always appended to end of used list.
//     When server processes queue entries, it always removes entries
//     beginning at front of used list.  Server apps may verify the
//     proper ordering of message entries by inspecting sequential message
//     id that is stored in each queue entry;  multi-block entries will have
//     same message id stored in each block.
//
//     In current implementation, CXQueue has been tested with multiple
//     clients, but only one server.
//
// History
//     Version 1.2 - 2005 January 17
//                 - Initial public release
//
// Public APIs:
//        NAME                      DESCRIPTION
//     ----------  -------------------------------------------------------
//     Close()     Close XQueue object and clean up.
//     Create()    Create() is called by the XQueue server to create the
//                 system-wide named XQueue object.
//     Dump()      Dump (TRACE) info about the XQueue object.
//     Open()      Open() is called by XQueue clients to open an existing
//                 XQueue object.
//     Read()      Read message from the queue.  Typically used only by
//                 XQueue servers.
//     Write()     Write message to the queue.  Typically used only by
//                 XQueue clients.
//
// License:
//     This software is released into the public domain.  You are free to use
//     it in any way you like, except that you may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////



#include <windows.h>
#include <stdio.h>
#include <crtdbg.h>
#include <tchar.h>
#include <stddef.h>
#include <ctime>
#define TRACE __noop


// determine number of elements in an array (not bytes)
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

#pragma warning(disable : 4127 4267 4312)		// conditional expression is constant
									// (needed for _ASSERTE)

///////////////////////////////////////////////////////////////////////////////
//
// If you do not want TRACE output from XQueue.cpp you can
// uncomment the following lines:
//
//#undef  TRACE
//#define TRACE ((void)0)
//

#include "XQueue.h"

///////////////////////////////////////////////////////////////////////////////
// prefixes for named objects
#define XQUEUE_NAME					_T("XQueue_")
#define WRITE_MUTEX_NAME			_T("XQueue_Write_")
#define CLIENT_WRITE_EVENT_NAME		_T("XQueue_ClientWrite_")
#define SERVER_NOTIFY_EVENT_NAME	_T("XQueue_ServerNotify_")
//add by zhangwei 20100917
#define WRITE_SEMAPHORE_NAME		_T("XQueue_Write_Free_")
#define READ_SEMAPHORE_NAME			_T("XQueue_Read_Used_")
#define READ_MUTEX_NAME				_T("XQueue_Read_")
///////////////////////////////////////////////////////////////////////////////
//
// CXQueue()
//
// Purpose:     Construct CXQueue object.
//
// Parameters:  None
//
// Returns:     None
//
CXQueue::CXQueue() :
	m_nBlockSize(XQUEUE_DEFAULT_BLOCK_SIZE),
	m_nDataSize(XQUEUE_DEFAULT_DATA_SIZE),
	m_nQueueSize(0),
	//m_bClient(TRUE),
	m_hWriteMutex(NULL),
	//m_hClientWriteEvent(NULL),
	//m_hServerNotifyEvent(NULL),
	m_pQueue(NULL),
	m_pVariableBlock(NULL),
	m_pDataBlock(NULL),
	m_hWriteSem(NULL),
	m_hReadSem(NULL),
	m_hReadMutex(NULL),
	m_bCreator(false)
{
	TRACE(_T("in CXQueue::CXQueue for thread %04X\n"), GetCurrentThreadId());

	ZeroMemory(m_szQueueName, sizeof(m_szQueueName));
}

///////////////////////////////////////////////////////////////////////////////
//
// ~CXQueue()
//
// Purpose:     Destroy CXQueue object.
//
// Parameters:  None
//
// Returns:     None
//
CXQueue::~CXQueue()
{
	TRACE(_T("in CXQueue::~CXQueue for thread %04X\n"), GetCurrentThreadId());

	Close();
}

///////////////////////////////////////////////////////////////////////////////
//
// Close()
//
// Purpose:     Close XQueue object and clean up.
//
// Parameters:  None
//
// Returns:     LONG - XQueue error code (see XQueue.h)
//
LONG CXQueue::Close()
{
	TRACE(_T("in CXQueue::Close\n"));

	Dump();

	m_MMF.Dump();

	if (IsOpen())
	{
		TRACE(_T("IsOpen\n"));
		//if (m_bClient)
		//{
		//	TRACE(_T("Client\n"));
		//	if (m_pVariableBlock->NoClients > 0)
		//		m_pVariableBlock->NoClients--;
		//}
		//else
		//{
		//	if (m_pVariableBlock->NoServers > 0)
		//		m_pVariableBlock->NoServers--;
		//}
	}

	if (m_pVariableBlock)
		m_pVariableBlock->Dump();

	m_MMF.UnMap();

	m_nQueueSize        = 0;
	m_pQueue            = NULL;
	m_pVariableBlock    = NULL;
	m_pDataBlock        = NULL;

	if (m_hWriteMutex)
		CloseHandle(m_hWriteMutex);
	m_hWriteMutex = NULL;

	//if (m_hClientWriteEvent)
	//	CloseHandle(m_hClientWriteEvent);
	//m_hClientWriteEvent = NULL;

	//if (m_hServerNotifyEvent)
	//	CloseHandle(m_hServerNotifyEvent);
	//m_hServerNotifyEvent = NULL;

	if (m_hWriteSem)
		CloseHandle(m_hWriteSem);
	m_hWriteSem = NULL;

	if (m_hReadSem)
		CloseHandle(m_hReadSem);
	m_hReadSem = NULL;

	if (m_hReadMutex)
		CloseHandle(m_hReadMutex);
	m_hReadMutex = NULL;

	ZeroMemory(m_szQueueName, sizeof(m_szQueueName));

	return XQUEUE_ERROR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//
// Create()
//
// Purpose:     Create() is called by the XQueue server to create the
//              system-wide named XQueue object.
//
// Parameters:  lpszQueueName - [in] name of XQueue object (must be specified)
//              nQueueSize    - [in] size of queue in blocks
//              nBlockSize    - [in] size of a block in bytes
//
// Returns:     LONG          - XQueue error code (see XQueue.h)
//
LONG CXQueue::Create(LPCTSTR lpszQueueName /*= NULL*/,
					 size_t nQueueSize /*= XQUEUE_DEFAULT_QUEUE_SIZE*/,
					 size_t nBlockSize /*= XQUEUE_DEFAULT_BLOCK_SIZE*/)
{
	TRACE(_T("in CXQueue::Create\n"));

	if (IsOpen())
	{
		TRACE(_T("ERROR:  already open\n"));
		_ASSERTE(FALSE);
		return XQUEUE_ERROR_CREATE_FAILED;
	}

	_ASSERTE(lpszQueueName && (lpszQueueName[0] != _T('\0')));
	if (!lpszQueueName || (lpszQueueName[0] == _T('\0')))
		return XQUEUE_ERROR_PARAMETER;

	ZeroMemory(m_szQueueName, sizeof(m_szQueueName));
	_tcscpy(m_szQueueName, XQUEUE_NAME);	// copy prefix
	int len = _tcslen(m_szQueueName);
	_tcsncat(m_szQueueName, lpszQueueName, _countof(m_szQueueName) - len - 1);

	m_nQueueSize = nQueueSize;
	if (m_nQueueSize == 0)
	{
		TRACE(_T("WARNING:  setting to default queue size\n"));
		m_nQueueSize = XQUEUE_DEFAULT_QUEUE_SIZE;
	}

	_ASSERTE(m_nQueueSize <= XQUEUE_MAX_QUEUE_SIZE);
	if (m_nQueueSize > XQUEUE_MAX_QUEUE_SIZE)
		return XQUEUE_ERROR_PARAMETER;

	_ASSERTE(m_nQueueSize >= XQUEUE_MIN_QUEUE_SIZE);
	if (m_nQueueSize < XQUEUE_MIN_QUEUE_SIZE)
		return XQUEUE_ERROR_PARAMETER;

	m_nBlockSize = nBlockSize;
	if (m_nBlockSize < XQUEUE_MIN_BLOCK_SIZE)
	{
		TRACE(_T("WARNING:  block size too small - setting to minimum block size\n"));
		m_nBlockSize = XQUEUE_MIN_BLOCK_SIZE;
	}

	// round up to next multiple of XQUEUE_MIN_BLOCK_SIZE
	m_nBlockSize = ((m_nBlockSize + (XQUEUE_MIN_BLOCK_SIZE-1)) /
						XQUEUE_MIN_BLOCK_SIZE) * XQUEUE_MIN_BLOCK_SIZE;

	m_nDataSize = m_nBlockSize - XQUEUE_DATA_VARIABLE_SIZE;

	TRACE(_T("m_nBlockSize=%d  m_nDataSize=%d\n"), m_nBlockSize, m_nDataSize);

	_ASSERTE((offsetof(XQUEUE_DATA_BLOCK, Data) + m_nDataSize) == m_nBlockSize);

	//m_bClient = FALSE;					// only servers call Create()

	m_nTotalBlocks = 1 + m_nQueueSize;	// include queue variable block

	LONG lRet = XQUEUE_ERROR_SUCCESS;

	// this is the server
	if (m_MMF.MapMemory(m_szQueueName, m_nTotalBlocks * m_nBlockSize))
	{
		m_MMF.Dump();

		lRet = CreateSyncObjects();		// create mutex and events

		if (lRet == XQUEUE_ERROR_SUCCESS)
		{
			m_pQueue = (LPBYTE) m_MMF.GetMappedAddress();
			_ASSERTE(m_pQueue);

			if (m_pQueue)
			{
				// set up queue variable block (first block in mmf)
				m_pVariableBlock = (XQUEUE_VARIABLE_BLOCK *) m_pQueue;
				ZeroMemory(m_pVariableBlock, m_nBlockSize);			// zero all variables

				// first data block is second block in mmf
				m_pDataBlock = m_pQueue + XQUEUE_FIRST_DATA_BLOCK_NO * m_nBlockSize;

				m_pVariableBlock->BlockSize      = m_nBlockSize;
				_ASSERTE(m_nBlockSize > XQUEUE_DATA_VARIABLE_SIZE);
				m_pVariableBlock->DataSize       = m_nBlockSize - XQUEUE_DATA_VARIABLE_SIZE;
				m_pVariableBlock->NoBlocks       = m_nTotalBlocks;	// includes variable block
				m_pVariableBlock->NoDataBlocks   = m_nQueueSize;	// no. data blocks (not
																	// including variable block
				m_pVariableBlock->FreeDataBlocks = m_nQueueSize;	// initially all are free
				m_pVariableBlock->ServerThreadId = (HANDLE) GetCurrentThreadId();
				m_pVariableBlock->MessageId      = 1;

				// set up list of free blocks
				XQUEUE_DATA_BLOCK *pXDB = NULL;
				XQUEUE_DATA_BLOCK *pPrevious = NULL;
				size_t back_link = XQUEUE_INVALID_BLOCK;

				for (size_t i = 0; i < m_nQueueSize; i++)
				{
					pXDB = (XQUEUE_DATA_BLOCK *) (m_pDataBlock + i * m_nBlockSize);
					pXDB->BlockNo     = i;
					pXDB->BackLink    = back_link;
					pXDB->ForwardLink = XQUEUE_INVALID_BLOCK;
					pXDB->ThreadId    = 0;
					pXDB->DataSize    = 0;
					pXDB->MessageId   = 0;
					pXDB->Data[0]     = 0;
					pXDB->Data[1]     = 0;

					back_link = i;

					if (pPrevious)
						pPrevious->ForwardLink = i;
					pPrevious = pXDB;
				}

				// free block list has all the blocks;
				// note:  data blocks are numbered from 0
				m_pVariableBlock->FreeListForwardLink = 0;
				m_pVariableBlock->FreeListBackLink    = m_nQueueSize - 1;

				// used block list is empty
				m_pVariableBlock->UsedListForwardLink = XQUEUE_INVALID_BLOCK;
				m_pVariableBlock->UsedListBackLink    = XQUEUE_INVALID_BLOCK;

				// this server is ready
				m_pVariableBlock->NoServers++;

				m_pVariableBlock->Dump();
			}
			else
			{
				TRACE(_T("ERROR:  GetMappedAddress returned NULL\n"));
				lRet = XQUEUE_ERROR_INTERNAL;
			}
		}
		else
		{
			_ASSERTE(FALSE);
			TRACE(_T("ERROR:  CreateSyncObjects failed\n"));
		}
	}
	else
	{
		TRACE(_T("ERROR:  cannot create mmf %s\n"), m_szQueueName);
		_ASSERTE(FALSE);
		lRet = XQUEUE_ERROR_CREATE_FAILED;
	}

	if (lRet != XQUEUE_ERROR_SUCCESS)
		Close();

	m_bCreator = true;
	return lRet;
}

///////////////////////////////////////////////////////////////////////////////
//
// Dump()
//
// Purpose:     Dump (TRACE) info about the XQueue object.
//
// Parameters:  None
//
// Returns:     None
//
void CXQueue::Dump()
{
	TRACE(_T("CXQueue dump:\n"));
	TRACE(_T("    m_hWriteMutex=0x%08X\n"),        m_hWriteMutex);
	//TRACE(_T("    m_hClientWriteEvent=0x%08X\n"),  m_hClientWriteEvent);
	//TRACE(_T("    m_hServerNotifyEvent=0x%08X\n"), m_hServerNotifyEvent);
	TRACE(_T("    m_hWriteSem=0x%08X\n"),  m_hWriteSem);
	TRACE(_T("    m_hReadSem=0x%08X\n"), m_hReadSem);
	TRACE(_T("    m_hReadMutex=0x%08X\n"), m_hReadMutex);
	TRACE(_T("    m_szQueueName=<%s>\n"),          m_szQueueName);
	TRACE(_T("    m_nQueueSize=%d\n"),             m_nQueueSize);
	TRACE(_T("    m_nTotalBlocks=%d\n"),           m_nTotalBlocks);
	TRACE(_T("    m_nBlockSize=%d\n"),             m_nBlockSize);
	TRACE(_T("    m_nDataSize=%d\n"),              m_nDataSize);

	//if (0 != m_pVariableBlock)
	//{
	//	XQUEUE_DATA_BLOCK * pBlock = 0;
	//	size_t nBlockNo = m_pVariableBlock->FreeListForwardLink;
	//	WriteLock();
	//	TRACE(_T("    FreeListForwardLink(%u)="),m_pVariableBlock->FreeDataBlocks );
	//	do
	//	{
	//		TRACE(_T("%X,"),nBlockNo );
	//		if (nBlockNo == XQUEUE_INVALID_BLOCK)
	//			break;

	//		pBlock = (XQUEUE_DATA_BLOCK *) (m_pDataBlock + nBlockNo * m_nBlockSize);
	//		nBlockNo = pBlock->BlockNo;
	//	} while (1);
	//	WriteUnlock();

	//	nBlockNo = m_pVariableBlock->UsedListForwardLink;
	//	ReadLock();
	//	TRACE(_T("    UsedListForwardLink(%u)="),m_pVariableBlock->UsedDataBlocks );
	//	do
	//	{
	//		TRACE(_T("%X,"),nBlockNo );
	//		if (nBlockNo == XQUEUE_INVALID_BLOCK)
	//			break;

	//		pBlock = (XQUEUE_DATA_BLOCK *) (m_pDataBlock + nBlockNo * m_nBlockSize);
	//		nBlockNo = pBlock->BlockNo;
	//	} while (1);
	//	ReadUnlock();
	//}
}

void CXQueue::Inf(char *pBuf, size_t nSize, int nMode)
{
	if (0 != m_pVariableBlock)
	{
		XQUEUE_DATA_BLOCK * pBlock = 0;
		size_t nBlockNo = 0;
		size_t nPos = 0;
		size_t nLen = 0;
		char szBuf[1024] = {0};
		long lRtn = XQUEUE_ERROR_SUCCESS;

		if (nMode == 0 || nMode == 2)
		{
			try
			{
				lRtn = WriteLock(1000);
				if (lRtn != XQUEUE_ERROR_SUCCESS)
					return;

				sprintf(szBuf, "FreeListForwardLink(%u):%u\r\n",m_pVariableBlock->FreeDataBlocks,m_pVariableBlock->FreeListForwardLink);
				nLen = strlen(szBuf);
				if (nPos + nLen > nSize)
				{
					WriteUnlock();
					return;
				}
				memcpy(pBuf + nPos, szBuf, nLen);
				nPos += nLen;		
				WriteUnlock();
			}
			catch(...)
			{
				WriteUnlock();
				return;
			}

			try
			{
				memset(szBuf, 0x00, sizeof(szBuf));	

				lRtn = ReadLock(1000);
				if (lRtn != XQUEUE_ERROR_SUCCESS)
					return;

				sprintf(szBuf, "UsedListForwardLink(%u):%u\r\n",m_pVariableBlock->UsedDataBlocks, m_pVariableBlock->UsedListForwardLink);
				nLen = strlen(szBuf);
				if (nPos + nLen > nSize)
				{
					ReadUnlock();
					return;
				}
				memcpy(pBuf + nPos, szBuf, nLen);
				nPos += nLen;
				ReadUnlock();
			}
			catch(...)
			{
				WriteUnlock();
				return;
			}

			long lPre = 0;
			lRtn = WaitForSemWrite(1000);
			if (lRtn == XQUEUE_ERROR_SUCCESS)
				ReleaseSemaphore(m_hWriteSem, 1, &lPre);
			memset(szBuf, 0x00, sizeof(szBuf));	
			sprintf(szBuf, "m_hWriteSem=%d\r\n",lPre);
			nLen = strlen(szBuf);
			if (nPos + nLen > nSize)
			{
				return;
			}
			memcpy(pBuf + nPos, szBuf, nLen);
			nPos += nLen;

			lRtn = WaitForSemRead(1000);
			if (lRtn == XQUEUE_ERROR_SUCCESS)
				ReleaseSemaphore(m_hReadSem, 1, &lPre);
			memset(szBuf, 0x00, sizeof(szBuf));	
			sprintf(szBuf, "m_hReadSem=%d\r\n",lPre);
			nLen = strlen(szBuf);
			if (nPos + nLen > nSize)
			{
				return;
			}
			memcpy(pBuf + nPos, szBuf, nLen);
			nPos += nLen;
		}

		if (nMode == 1 || nMode == 2)
		{
			try
			{
				lRtn = WriteLock(1000);
				if (lRtn != XQUEUE_ERROR_SUCCESS)
					return;

				sprintf(szBuf, "FreeListForwardLink(%u)=\r\n",m_pVariableBlock->FreeDataBlocks);
				nLen = strlen(szBuf);
				if (nPos + nLen > nSize)
				{
					WriteUnlock();
					return;
				}
				memcpy(pBuf + nPos, szBuf, nLen);
				nPos += nLen;
			
				nBlockNo = m_pVariableBlock->FreeListForwardLink;
				memset(szBuf, 0x00, sizeof(szBuf));		
				do
				{
					sprintf(szBuf, "0x%X,",nBlockNo );
					nLen = strlen(szBuf);
					if (nPos + nLen > nSize)
					{
						WriteUnlock();
						return;
					}
					memcpy(pBuf + nPos, szBuf, nLen);
					nPos += nLen;

					if (nBlockNo == XQUEUE_INVALID_BLOCK || nBlockNo >= m_nQueueSize)
						break;

					pBlock = (XQUEUE_DATA_BLOCK *) (m_pDataBlock + nBlockNo * m_nBlockSize);
					nBlockNo = pBlock->ForwardLink;
				} while (1);
				WriteUnlock();
			}
			catch(...)
			{
				WriteUnlock();
				return;
			}

			try
			{
				memset(szBuf, 0x00, sizeof(szBuf));	

				lRtn = ReadLock(1000);
				if (lRtn != XQUEUE_ERROR_SUCCESS)
					return;

				sprintf(szBuf, "\r\nUsedListForwardLink(%u)=\r\n",m_pVariableBlock->UsedDataBlocks );
				nLen = strlen(szBuf);
				if (nPos + nLen > nSize)
				{
					ReadUnlock();
					return;
				}
				memcpy(pBuf + nPos, szBuf, nLen);
				nPos += nLen;

				nBlockNo = m_pVariableBlock->UsedListForwardLink;
				memset(szBuf, 0x00, sizeof(szBuf));		
				do
				{
					sprintf(szBuf, "%X,",nBlockNo );
					nLen = strlen(szBuf);
					if (nPos + nLen > nSize)
					{
						ReadUnlock();
						return;
					}
					memcpy(pBuf + nPos, szBuf, nLen);
					nPos += nLen;

					if (nBlockNo == XQUEUE_INVALID_BLOCK)
						break;

					pBlock = (XQUEUE_DATA_BLOCK *) (m_pDataBlock + nBlockNo * m_nBlockSize);
					nBlockNo = pBlock->ForwardLink;
				} while (1);
				ReadUnlock();
			}
			catch(...)
			{
				WriteUnlock();
				return;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//
// Open()
//
// Purpose:     Open() is called by XQueue clients to open an existing XQueue
//              object.
//
// Parameters:  lpszQueueName - [in] name of XQueue object (must be specified)
//
// Returns:     LONG          - XQueue error code (see XQueue.h)
//
LONG CXQueue::Open(LPCTSTR lpszQueueName)
{
	TRACE(_T("in CXQueue::Open -------\n"));

	if (IsOpen())
	{
		TRACE(_T("ERROR:  already open\n"));
		_ASSERTE(FALSE);
		return XQUEUE_ERROR_OPEN_FAILED;
	}

	_ASSERTE(lpszQueueName && (lpszQueueName[0] != _T('\0')));
	if (!lpszQueueName || (lpszQueueName[0] == _T('\0')))
		return XQUEUE_ERROR_PARAMETER;

	ZeroMemory(m_szQueueName, sizeof(m_szQueueName));
	_tcscpy(m_szQueueName, XQUEUE_NAME);	// copy prefix
	int len = _tcslen(m_szQueueName);
	_tcsncat(m_szQueueName, lpszQueueName, _countof(m_szQueueName) - len - 1);

	LONG lRet = XQUEUE_ERROR_SUCCESS;

	if (m_MMF.MapExistingMemory(m_szQueueName))
	{
		m_MMF.Dump();

		lRet = OpenSyncObjects();

		if (lRet == XQUEUE_ERROR_SUCCESS)
		{
			//m_bClient = TRUE;
			m_nQueueSize = 0;

			m_pQueue = (LPBYTE) m_MMF.GetMappedAddress();
			_ASSERTE(m_pQueue);

			if (m_pQueue)
			{
				m_pVariableBlock  = (XQUEUE_VARIABLE_BLOCK *) m_pQueue;

				// set the client block sizes to the same size as the server
				m_nBlockSize   = m_pVariableBlock->BlockSize;
				m_nDataSize    = m_pVariableBlock->DataSize;
				TRACE(_T("m_nBlockSize=%d  m_nDataSize=%d\n"), m_nBlockSize, m_nDataSize);

				m_nQueueSize   = m_pVariableBlock->NoDataBlocks;
				m_nTotalBlocks = m_pVariableBlock->NoBlocks;
				m_pDataBlock   = m_pQueue + XQUEUE_FIRST_DATA_BLOCK_NO * m_nBlockSize;

				m_pVariableBlock->NoClients++;

				m_pVariableBlock->Dump();
			}
			else
			{
				TRACE(_T("ERROR:  GetMappedAddress returned NULL\n"));
				lRet = XQUEUE_ERROR_INTERNAL;
			}
		}
	}
	else
	{
		TRACE(_T("ERROR:  cannot open mmf %s\n"), m_szQueueName);
		lRet = XQUEUE_ERROR_OPEN_FAILED;
	}

	if (lRet != XQUEUE_ERROR_SUCCESS)
		Close();

	return lRet;
}

///////////////////////////////////////////////////////////////////////////////
//
// Read()
//
// Purpose:     Read message from the queue.  Typically used only by XQueue
//              servers.
//
// Parameters:  lpData       - [out] Pointer to the buffer that receives the
//                             data read from the file
//              lpcbSize     - [in/out] size in bytes of lpData;  if lpData
//                             is NULL, the size of the required buffer will
//                             returned in lpcbSize.
//              lpnMessageId - [out] the message id contained in the queue block
//              pdwThreadId  - [out] the thread id of the thread that queued
//                             the message
//              dwTimeOut    - [in] specifies the time-out interval, in
//                             milliseconds
//
// Returns:     LONG         - XQueue error code (see XQueue.h)
//
LONG CXQueue::Read(LPBYTE   lpData,
				   size_t * lpcbSize,
				   size_t * lpnMessageId,
				   DWORD  * pdwThreadId,
				   DWORD    dwTimeOut /*= INFINITE*/)
{
	_ASSERTE(lpcbSize);
	if (!lpcbSize)
		return XQUEUE_ERROR_PARAMETER;

	_ASSERTE(lpnMessageId);
	if (!lpnMessageId)
		return XQUEUE_ERROR_PARAMETER;

	_ASSERTE(m_pVariableBlock);
	if (!m_pVariableBlock)
		return XQUEUE_ERROR_NOT_OPEN;

	// begin modify by zhangwei 20100917
	//*lpnMessageId = 0;

	//LONG lRet = XQUEUE_ERROR_NO_DATA;

	//if ((m_pVariableBlock) &&
	//	(m_pVariableBlock->FreeDataBlocks < m_pVariableBlock->NoDataBlocks))
	//{
	//	size_t block_no = m_pVariableBlock->UsedListForwardLink;
	//	if (block_no != XQUEUE_INVALID_BLOCK)
	//	{
	//		XQUEUE_DATA_BLOCK *pXDB =
	//			(XQUEUE_DATA_BLOCK *) (m_pDataBlock + block_no * m_nBlockSize);

	//		*lpnMessageId = pXDB->MessageId;
	//		if (pdwThreadId)
	//			*pdwThreadId  = pXDB->ThreadId;

	//		if (!lpData)
	//		{
	//			// caller just wants the size
	//			*lpcbSize = GetEntrySize(pXDB);
	//			TRACE(_T("GetEntrySize returned %d for block %d\n"),
	//			*lpcbSize, block_no);
	//			lRet = XQUEUE_ERROR_SUCCESS;
	//		}
	//		else
	//		{
	//			// caller wants the data
	//			lRet = ReadMMF(lpData, lpcbSize, dwTimeOut);
	//		}
	//	}
	//}

	//if (lRet == XQUEUE_ERROR_NO_DATA)
	//	*lpcbSize = 0;

	//TRACE(_T("Read returning %d bytes -----\n"), *lpcbSize);

	//return lRet;
	// end modify by zhangwei 20100917

	if (lpnMessageId)
		*lpnMessageId = 0;
	if (pdwThreadId)
		*pdwThreadId = 0;

	long lRet = XQUEUE_ERROR_SUCCESS;
	lRet = WaitForSemRead(dwTimeOut);
	if (lRet != XQUEUE_ERROR_SUCCESS)
		return XQUEUE_ERROR_TIMEOUT;

	lRet = ReadLock();
	if (lRet != XQUEUE_ERROR_SUCCESS)
		return XQUEUE_ERROR_TIMEOUT;

	_ASSERTE(m_pDataBlock);
	_ASSERTE(m_pVariableBlock);
	size_t nBlockNo = m_pVariableBlock->UsedListForwardLink;
	_ASSERTE(nBlockNo < m_nQueueSize);
	if (nBlockNo == XQUEUE_INVALID_BLOCK)
	{
		ReadUnlock();
		return XQUEUE_ERROR_NO_DATA;
	}

	XQUEUE_DATA_BLOCK * pReadBlockFirst = (XQUEUE_DATA_BLOCK *) (m_pDataBlock + nBlockNo * m_nBlockSize);	
	XQUEUE_DATA_BLOCK * pReadBlockLast = 0;
	if ((pReadBlockFirst->DataSize & XQUEUE_CONTINUATION_FLAG) == 0)
	{
		lRet = ReadSingleBlock(lpData, lpcbSize, pReadBlockFirst);
		ReadUnlock();

		if (lRet == XQUEUE_ERROR_SUCCESS)
		{
			WriteLock();
			AddFree(pReadBlockFirst);
			WriteUnlock();

			ReleaseSemaphore(m_hWriteSem,1,NULL);
		}
	}
	else
	{
		lRet = GetUsedListForward(*lpcbSize, pReadBlockFirst, &pReadBlockLast);
		ReadUnlock();

		if (lRet == XQUEUE_ERROR_SUCCESS)
		{
			size_t nBlocks = ReadMultiBlock(lpData, lpcbSize, pReadBlockFirst);
			ReleaseSemaphore(m_hWriteSem,nBlocks,NULL);
		}
	}		
	return lRet;
}

///////////////////////////////////////////////////////////////////////////////
//
// Write()
//
// Purpose:     Write string to the queue.  Typically used only by XQueue
//              clients.
//
// Parameters:  lpszString - [in] Pointer to the buffer that contains the
//                           nul-terminated string to queue
//              dwTimeOut  - [in] specifies the time-out interval, in
//                           milliseconds
//
// Returns:     LONG       - XQueue error code (see XQueue.h)
//
LONG CXQueue::Write(LPCTSTR lpszString,
					DWORD dwTimeOut /*= INFINITE*/)
{
	if (!IsOpen())
		return XQUEUE_ERROR_NOT_OPEN;

	_ASSERTE(lpszString);
	if (!lpszString)
		return XQUEUE_ERROR_PARAMETER;

	size_t nDataSize = (_tcslen(lpszString)+1) * sizeof(TCHAR);	// no. of bytes

	return Write((LPBYTE) lpszString, nDataSize, dwTimeOut);
}

///////////////////////////////////////////////////////////////////////////////
//
// Write()
//
// Purpose:     Write byte data to the queue.  Typically used only by XQueue
//              clients.
//
// Parameters:  lpData    - [in] Pointer to the buffer that contains the
//                          data to queue
//              nDataSize - [in] number of bytes of data in lpData
//              dwTimeOut - [in] specifies the time-out interval, in
//                          milliseconds
//
// Returns:     LONG      - XQueue error code (see XQueue.h)
//
LONG CXQueue::Write(LPBYTE lpData,
					size_t nDataSize,
					DWORD dwTimeOut /*= INFINITE*/)
{
	TRACE(_T("in CXQueue::Write:  %d bytes -----\n"), nDataSize);

	_ASSERTE(lpData);
	_ASSERTE(nDataSize > 0);
	if (!lpData || (nDataSize == 0))
		return XQUEUE_ERROR_PARAMETER;

	// check size of write
	_ASSERTE(nDataSize < (m_nBlockSize * (m_nQueueSize-1)));
	if (nDataSize >= (m_nBlockSize * (m_nQueueSize-1)))
		return XQUEUE_ERROR_PARAMETER;

	_ASSERTE(m_pVariableBlock);
	if (!m_pVariableBlock)
		return XQUEUE_ERROR_NOT_OPEN;

	//begin modify by zhangwei 20100917
	//if (m_pVariableBlock->NoServers == 0)
	//{
	//	TRACE(_T("ERROR:  no server\n"));
	//	return XQUEUE_ERROR_NO_SERVER;
	//}

	// wait until no _new_ client writes are allowed - it doesn't
	// matter that there are still client writes going on; we just want
	// to prevent any new ones, so the server will have a chance to
	// free some blocks (if necessary).  Note:  this is auto reset event.

	//modify by zhangwei 20100917
	//LONG lWaitResult = WaitForQueueIdle(INFINITE);

	//if (lWaitResult != XQUEUE_ERROR_SUCCESS)
	//{
	//	TRACE(_T("ERROR:  WaitForQueueIdle failed for thread %X -----\n"),
	//		GetCurrentThreadId());
	//	_ASSERTE(FALSE);
	//	return lWaitResult;
	//}
	//end modify by zhangwei 20100917

	// determine how many blocks we will need
	size_t nBlocks = CalculateNoBlocks(nDataSize);

	//begin modify by zhangwei 20100917
	//BOOL bFound = TRUE;
	//LONG lRet = XQUEUE_ERROR_SUCCESS;

	// are there enough free blocks?	
	//if (m_pVariableBlock->FreeDataBlocks < nBlocks)
	//{
	//	// no, we must wait until server frees some

	//	bFound = FALSE;

	//	if (dwTimeOut == 0)
	//	{
	//		TRACE(_T("ERROR:  not enough free blocks\n"));
	//		lRet = XQUEUE_ERROR_NO_FREE;
	//	}
	//	else
	//	{
	//		DWORD dwFreeTimeOut = dwTimeOut;
	//		DWORD dwSpinTime = 1;

	//		// loop until server frees some blocks
	//		do
	//		{
	//			Sleep(dwSpinTime);

	//			if ((lRet == XQUEUE_ERROR_SUCCESS) &&
	//				(m_pVariableBlock->FreeDataBlocks >= nBlocks))
	//			{
	//				// there are now enough free blocks, we can write
	//				bFound = TRUE;
	//				break;
	//			}

	//			if (dwFreeTimeOut > dwSpinTime)
	//				dwFreeTimeOut -= dwSpinTime;
	//			else
	//				dwFreeTimeOut = 0;

	//		} while (dwFreeTimeOut > 0);
	//	}
	//}

	//if (!bFound)
	//{
	//	// timeout expired, not enough free blocks
	//	TRACE(_T("ERROR:  timeout waiting for enough free blocks\n"));
	//	lRet = XQUEUE_ERROR_TIMEOUT;
	//}
	//else
	//{
	//	// there are enough free blocks

	//	lRet = Lock(dwTimeOut);	//===================================

	//	if (lRet == XQUEUE_ERROR_SUCCESS)
	//	{
	//		lRet = WriteToMMF(lpData, nDataSize);

	//		if (lRet == XQUEUE_ERROR_SUCCESS)
	//		{
	//			// update some XQueue statistics
	//			m_pVariableBlock->NoWrites++;

	//			if (nDataSize > m_pVariableBlock->MaxDataSize)
	//				m_pVariableBlock->MaxDataSize = nDataSize;

	//			// increment message id (all blocks within a message
	//			// will have the same id)
	//			m_pVariableBlock->MessageId++;
	//		}

	//		Unlock();			//===================================
	//	}

	//	if (lRet == XQUEUE_ERROR_SUCCESS)
	//	{
	//		// tell server something has been queued
	//		if (m_hServerNotifyEvent)
	//			SetEvent(m_hServerNotifyEvent);
	//	}
	//}

	// allow other clients to write
	
	//SetEvent(m_hClientWriteEvent);
	//end modify by zhangwei 20100917

	
	//begin add by zhangwei 20100917	
	if (nBlocks > m_nQueueSize)
		return XQUEUE_ERROR_TOO_BIG;

	long lRtnWrite = XQUEUE_ERROR_SUCCESS;
	if (nBlocks > 1)
		lRtnWrite = WriteMultiBlock(lpData, nDataSize, dwTimeOut, nBlocks);	
	else
		lRtnWrite = WriteSingleBlock(lpData, nDataSize, dwTimeOut);		

	if (lRtnWrite == XQUEUE_ERROR_SUCCESS)
		ReleaseSemaphore(m_hReadSem,1,NULL);
	return lRtnWrite;
	//end  add by zhangwei 20100917
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//  PRIVATE FUNCTIONS
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
// Add() - private function
//
// Purpose:     Add a block to list
//
// Parameters:  ForwardLink - [out] reference to forward link
//              BackLink    - [out] reference to backward link
//              BlockCount  - [in/out] number of blocks already in list
//              pXDB        - [in] pointer to Xqueue data block
//
// Returns:     None
//
void CXQueue::Add(size_t &ForwardLink,
				  size_t &BackLink,
				  size_t &BlockCount,
				  XQUEUE_DATA_BLOCK * pXDB)
{
	_ASSERTE(pXDB);
	if (!pXDB)
		return;

	// make sure list isn't already full - caller should have checked this
	_ASSERTE(BlockCount < m_pVariableBlock->NoDataBlocks);
	if (BlockCount >= m_pVariableBlock->NoDataBlocks)
	{
		TRACE(_T("ERROR:  list full\n"));
		return;
	}

	// add block to end of list
	size_t block_no = BackLink;
	XQUEUE_DATA_BLOCK * pLast = NULL;
	if (block_no != XQUEUE_INVALID_BLOCK)
		pLast = (XQUEUE_DATA_BLOCK *) (m_pDataBlock + block_no * m_nBlockSize);

	// at this point pLast is NULL if list is empty,
	// or pLast points to last block in list

	if (pLast == NULL)
	{
		// used list is empty
		ForwardLink = pXDB->BlockNo;
		BackLink    = pXDB->BlockNo;
		pXDB->ForwardLink = XQUEUE_INVALID_BLOCK;
		pXDB->BackLink    = XQUEUE_INVALID_BLOCK;
	}
	else
	{
		// used list not empty, pLast points to last block;
		// add block to end of list
		pLast->ForwardLink = pXDB->BlockNo;
		pXDB->BackLink     = pLast->BlockNo;
		pXDB->ForwardLink  = XQUEUE_INVALID_BLOCK;
		BackLink = pXDB->BlockNo;
	}

	// bump count for this list
	BlockCount++;
	if (BlockCount > m_pVariableBlock->NoDataBlocks)
		BlockCount = m_pVariableBlock->NoDataBlocks;
}

///////////////////////////////////////////////////////////////////////////////
//
// AddFree() - private function
//
// Purpose:     Add a block to the free list
//
// Parameters:  pXDB - [in] pointer to Xqueue data block
//
// Returns:     None
//
void CXQueue::AddFree(XQUEUE_DATA_BLOCK * pXDB)
{
	Add(m_pVariableBlock->FreeListForwardLink,
		m_pVariableBlock->FreeListBackLink,
		m_pVariableBlock->FreeDataBlocks,
		pXDB);	
}

///////////////////////////////////////////////////////////////////////////////
//
// AddUsed() - private function
//
// Purpose:     Add a block to the used list
//
// Parameters:  pXDB - [in] pointer to Xqueue data block
//
// Returns:     None
//
void CXQueue::AddUsed(XQUEUE_DATA_BLOCK * pXDB)
{
	Add(m_pVariableBlock->UsedListForwardLink,
		m_pVariableBlock->UsedListBackLink,
		m_pVariableBlock->UsedDataBlocks,
		pXDB);
}

//
///////////////////////////////////////////////////////////////////////////////
//
// AddUsedListBack() - private function
//
// Purpose:     Add a block list to the used list by the last one
//
// Parameters:  pXDB - [in] pointer to Xqueue data block
//
// Returns:     None
//
void CXQueue::AddUsedList(XQUEUE_DATA_BLOCK * pWriteBlockFirst, XQUEUE_DATA_BLOCK * pWriteBlockLast)
{
	_ASSERTE(pWriteBlockLast && pWriteBlockFirst);
	if (!pWriteBlockLast || !pWriteBlockFirst)
		return;

	size_t nOldBlockLast = m_pVariableBlock->UsedListBackLink;
	if (nOldBlockLast != XQUEUE_INVALID_BLOCK)
	{
		XQUEUE_DATA_BLOCK * pOldLast = (XQUEUE_DATA_BLOCK *) (m_pDataBlock + nOldBlockLast * m_nBlockSize);
		pOldLast->ForwardLink = pWriteBlockFirst->BlockNo;
		pWriteBlockFirst->BackLink = nOldBlockLast;
		m_pVariableBlock->UsedListBackLink = pWriteBlockLast->BlockNo;
	}
	else
	{
		m_pVariableBlock->UsedListBackLink = pWriteBlockLast->BlockNo;
		m_pVariableBlock->UsedListForwardLink = pWriteBlockFirst->BlockNo;
	}

}

//

///////////////////////////////////////////////////////////////////////////////
//
// GetBlock() - private function
//
// Purpose:     Get a block from a list, delink it from the list
//
// Parameters:  ForwardLink - [out] reference to forward link
//              BackLink    - [out] reference to backward link
//              BlockCount  - [in/out] number of blocks in list
//
// Returns:     XQUEUE_DATA_BLOCK *  - pointer to XQueue data block, or NULL
//
XQUEUE_DATA_BLOCK * CXQueue::GetBlock(size_t &ForwardLink,
									  size_t &BackLink,
									  size_t &BlockCount)
{
	// check if list is empty
	if (BlockCount == 0)
		return NULL;

	// check list head forward link
	size_t block_no = ForwardLink;
	if (block_no == XQUEUE_INVALID_BLOCK)
		return NULL;

	_ASSERTE(m_pDataBlock);
	_ASSERTE(m_pVariableBlock);
	_ASSERTE(block_no < m_nQueueSize);

	XQUEUE_DATA_BLOCK * pXDB =
		(XQUEUE_DATA_BLOCK *) (m_pDataBlock + block_no * m_nBlockSize);

	// check if block is valid
	_ASSERTE(pXDB->BlockNo == block_no);
	if (pXDB->BlockNo != block_no)
	{
		TRACE(_T("ERROR:  bad data block %d\n"), block_no);
		return NULL;
	}

	size_t new_first_block = pXDB->ForwardLink;
	ForwardLink = new_first_block;

	if (new_first_block == XQUEUE_INVALID_BLOCK)
	{
		BackLink = XQUEUE_INVALID_BLOCK;
	}
	else
	{
		XQUEUE_DATA_BLOCK * pFirst =
			(XQUEUE_DATA_BLOCK *) (m_pDataBlock + new_first_block * m_nBlockSize);

		pFirst->BackLink = XQUEUE_INVALID_BLOCK;
	}

	// one less entry in list
	if (BlockCount > 0)
		BlockCount--;

	// store thread id for debugging
	pXDB->ThreadId = GetCurrentThreadId();

	TRACE(_T("GetBlock returning %d\n"), block_no);

	return pXDB;
}

///////////////////////////////////////////////////////////////////////////////
//
// GetFreeBlock() - private function
//
// Purpose:     Get a block from the free list
//
// Parameters:  None
//
// Returns:     XQUEUE_DATA_BLOCK *  - pointer to XQueue data block, or NULL
//
XQUEUE_DATA_BLOCK * CXQueue::GetFreeBlock()
{
	return GetBlock(m_pVariableBlock->FreeListForwardLink,
					m_pVariableBlock->FreeListBackLink,
				m_pVariableBlock->FreeDataBlocks);
		
}

///////////////////////////////////////////////////////////////////////////////
//
// GetUsedBlock() - private function
//
// Purpose:     Get a block from the used list
//
// Parameters:  None
//
// Returns:     XQUEUE_DATA_BLOCK *  - pointer to XQueue data block, or NULL
//
XQUEUE_DATA_BLOCK * CXQueue::GetUsedBlock()
{
	return GetBlock(m_pVariableBlock->UsedListForwardLink,
					m_pVariableBlock->UsedListBackLink,
					m_pVariableBlock->UsedDataBlocks);
}

///////////////////////////////////////////////////////////////////////////////
//
// GetEntrySize() - private function
//
// Purpose:     Get the size in bytes of a block (including any continuation
//              blocks)
//
// Parameters:  pXDB   - [in] pointer to queue block
//
// Returns:     size_t - size in bytes
//
size_t CXQueue::GetEntrySize(XQUEUE_DATA_BLOCK *pXDB)
{
	_ASSERTE(pXDB);
	if (!pXDB)
	{
		TRACE(_T("ERROR:  pXDB is NULL\n"));
		return 0;
	}

	size_t nSize = 0;

	for(;;)
	{
		nSize += pXDB->DataSize & 0x7FFFFFFF;	// mask off continuation bit

		// is there another block?
		if ((pXDB->DataSize & XQUEUE_CONTINUATION_FLAG) == 0)
			break;

		size_t block_no = pXDB->ForwardLink;
		if (block_no == XQUEUE_INVALID_BLOCK)
			break;

		pXDB = (XQUEUE_DATA_BLOCK *) (m_pDataBlock + block_no * m_nBlockSize);
	}

	return nSize;
}

///////////////////////////////////////////////////////////////////////////////
//
// ReadMMF() - private function
//
// Purpose:     Read memory-mapped file
//
// Parameters:  lpData       - [out] Pointer to the buffer that receives the
//                             data read from the file
//              lpcbSize     - [in/out] size in bytes of lpData;  if lpData
//                             is NULL, the size of the required buffer will
//                             returned in lpcbSize.
//              dwTimeOut    - [in] specifies the time-out interval, in
//                             milliseconds
//
// Returns:     LONG         - XQueue error code (see XQueue.h)
//
// begin modify by zhangwei 20100917
//LONG CXQueue::ReadMMF(LPBYTE lpData,
//					  size_t * lpcbSize,
//					  DWORD dwTimeOut)
//{
//	LONG lRet = XQUEUE_ERROR_SUCCESS;
//
//	// verify parameters
//	if ((lpData == NULL) ||
//		(lpcbSize == NULL) ||
//		(*lpcbSize == 0) ||
//		IsBadWritePtr(lpData, *lpcbSize))
//	{
//		TRACE(_T("ERROR: bad parameters\n"));
//		_ASSERTE(FALSE);
//		*lpcbSize = 0;
//		return XQUEUE_ERROR_PARAMETER;
//	}
//
//	size_t block_no = m_pVariableBlock->UsedListForwardLink;
//	if (block_no == XQUEUE_INVALID_BLOCK)
//	{
//		*lpcbSize = 0;
//		return XQUEUE_ERROR_NO_DATA;
//	}
//
//	_ASSERTE(m_pDataBlock);
//	_ASSERTE(m_pVariableBlock);
//	_ASSERTE(block_no < m_nQueueSize);
//
//	XQUEUE_DATA_BLOCK * pXDB = NULL;
//
//	size_t nSize = 0;
//
//	while (nSize < *lpcbSize)
//	{
//		if (block_no == XQUEUE_INVALID_BLOCK)
//			break;
//
//		pXDB = (XQUEUE_DATA_BLOCK *) (m_pDataBlock + block_no * m_nBlockSize);
//
//		// check if block is valid
//		_ASSERTE(pXDB->BlockNo == block_no);
//		if (pXDB->BlockNo != block_no)
//		{
//			TRACE(_T("ERROR:  bad data block %d\n"), block_no);
//			nSize = 0;
//			lRet = XQUEUE_ERROR_INTERNAL;
//			break;
//		}
//
//		size_t nBytesToCopy = pXDB->DataSize & 0x7FFFFFFF;	// mask off continuation bit
//		if ((nSize + nBytesToCopy) > *lpcbSize)
//		{
//			// too much data for buffer - only copy what will fit
//			nBytesToCopy = *lpcbSize - nSize;
//			lRet = XQUEUE_ERROR_MORE_DATA;
//		}
//		nSize += nBytesToCopy;
//
//		if (nBytesToCopy > 0)
//			memcpy(lpData, pXDB->Data, nBytesToCopy);
//
//		lpData += nBytesToCopy;
//
//		if (lRet == XQUEUE_ERROR_MORE_DATA)
//			break;
//
//		// is there another block?
//		if ((pXDB->DataSize & XQUEUE_CONTINUATION_FLAG) == 0)
//			break;
//
//		block_no = pXDB->ForwardLink;
//	}
//
//	// if there's room left, append some zeros
//	size_t nSize2 = nSize;
//	while ((nSize2 < *lpcbSize) && ((nSize2 - nSize) < 4))
//	{
//		*lpData++ = 0;
//		nSize2++;
//	}
//
//	if (lRet == XQUEUE_ERROR_MORE_DATA)
//	{
//		*lpcbSize = GetEntrySize(pXDB);
//	}
//	else if (lRet == XQUEUE_ERROR_INTERNAL)
//	{
//		*lpcbSize = 0;
//	}
//	else
//	{
//		*lpcbSize = nSize;
//
//		lRet = Lock(dwTimeOut);	//===================================
//
//		if (lRet == XQUEUE_ERROR_SUCCESS)
//		{
//			ReleaseEntry();
//
//			m_pVariableBlock->NoReads++;
//
//			//modify by zhangwei if lock fail then do nothing
//			Unlock();				//===================================
//		}
//	}
//
//	return lRet;
//}
// end modify by zhangwei 20100917


// begin add by zhangwei 20100917
LONG CXQueue::GetUsedListForward(size_t nDataSize, XQUEUE_DATA_BLOCK * pReadBlockFirst, XQUEUE_DATA_BLOCK ** ppReadBlockLast)
{
	long lRet = XQUEUE_ERROR_SUCCESS;
	
	size_t nCount = 0;
	size_t nSize = 0;
	size_t nBlockNo = 0;
	XQUEUE_DATA_BLOCK * pReadBlockLast = pReadBlockFirst;
	do
	{
		size_t nBytesToCopy = pReadBlockLast->DataSize & 0x7FFFFFFF;
		if ((nSize + nBytesToCopy) > nDataSize)
		{
			// too much data for buffer
			lRet = XQUEUE_ERROR_MORE_DATA;
			break;
		}
		
		nCount++;
		nSize += nBytesToCopy;
		
		if ((pReadBlockLast->DataSize & XQUEUE_CONTINUATION_FLAG) == 0)
		{
			break;
		}
		else
		{
			nBlockNo = pReadBlockLast->ForwardLink;
			if (nBlockNo == XQUEUE_INVALID_BLOCK)
				break;

			pReadBlockLast = (XQUEUE_DATA_BLOCK *) (m_pDataBlock + nBlockNo * m_nBlockSize);
		}
	} while (1);
	*ppReadBlockLast = pReadBlockLast;

	if (lRet != XQUEUE_ERROR_SUCCESS)
		return lRet;

	m_pVariableBlock->UsedListForwardLink = pReadBlockLast->ForwardLink;
	if (pReadBlockLast->ForwardLink != XQUEUE_INVALID_BLOCK)
	{
		XQUEUE_DATA_BLOCK * pNewBlockFirst = (XQUEUE_DATA_BLOCK *) (m_pDataBlock + pReadBlockLast->ForwardLink * m_nBlockSize);
		pNewBlockFirst->BackLink = XQUEUE_INVALID_BLOCK;
		pReadBlockLast->ForwardLink = XQUEUE_INVALID_BLOCK;
	}
	else
	{
		m_pVariableBlock->UsedListBackLink = XQUEUE_INVALID_BLOCK;
	}
	m_pVariableBlock->UsedDataBlocks -= nCount;
	return lRet;
}

void CXQueue::AddFreeList(XQUEUE_DATA_BLOCK * pReadBlockFirst, XQUEUE_DATA_BLOCK * pReadBlockLast)
{
	_ASSERTE(pReadBlockFirst && pReadBlockLast);
	if (!pReadBlockFirst || !pReadBlockLast)
		return;

	if (m_pVariableBlock->FreeListBackLink != XQUEUE_INVALID_BLOCK)
	{
		XQUEUE_DATA_BLOCK * pOldLast = (XQUEUE_DATA_BLOCK *)(m_pDataBlock + m_pVariableBlock->FreeListBackLink * m_nBlockSize);
		pOldLast->ForwardLink = pReadBlockFirst->BlockNo;
		pReadBlockFirst->BackLink = m_pVariableBlock->FreeListBackLink;
		m_pVariableBlock->FreeListBackLink = pReadBlockLast->BlockNo;
		pReadBlockLast->ForwardLink = XQUEUE_INVALID_BLOCK;
	}
	else
	{
		m_pVariableBlock->FreeListForwardLink = pReadBlockFirst->BlockNo;
		m_pVariableBlock->FreeListBackLink = pReadBlockLast->BlockNo;
		pReadBlockFirst->BackLink = XQUEUE_INVALID_BLOCK;
		pReadBlockLast->ForwardLink = XQUEUE_INVALID_BLOCK;
	}
}

size_t CXQueue::ReadMultiBlock(LPBYTE lpData, size_t * lpcbSize, XQUEUE_DATA_BLOCK * pReadBlockFirst)
{
	XQUEUE_DATA_BLOCK * pReadBlockLast = pReadBlockFirst;
	size_t nCount = 0;
	size_t nSize = 0;
	size_t nBlockNo = 0;
	do
	{
		size_t nBytesToCopy = pReadBlockLast->DataSize & 0x7FFFFFFF;
		if (nBytesToCopy > 0)
			memcpy(lpData, pReadBlockLast->Data, nBytesToCopy);

		nCount++;
		nSize += nBytesToCopy;
		if ((pReadBlockLast->DataSize & XQUEUE_CONTINUATION_FLAG) == 0)
		{
			break;
		}
		else
		{
			pReadBlockLast->DataSize = 0;
			nBlockNo = pReadBlockLast->ForwardLink;
			lpData += nBytesToCopy;
			pReadBlockLast = (XQUEUE_DATA_BLOCK *) (m_pDataBlock + nBlockNo * m_nBlockSize);
		}
	} while (1);
	*lpcbSize = nSize;

	WriteLock();
	AddFreeList(pReadBlockFirst, pReadBlockLast);
	m_pVariableBlock->FreeDataBlocks += nCount;
	WriteUnlock();	
	return nCount;
}

LONG CXQueue::ReadSingleBlock(LPBYTE lpData, size_t * lpcbSize, XQUEUE_DATA_BLOCK * pXDB)
{
	size_t nBytesToCopy = pXDB->DataSize & 0x7FFFFFFF;
	if (nBytesToCopy > *lpcbSize)
		return XQUEUE_ERROR_MORE_DATA;

	memcpy(lpData, pXDB->Data, nBytesToCopy);
	pXDB->DataSize = 0;
	*lpcbSize = nBytesToCopy;

	size_t nNewFirstBlock = pXDB->ForwardLink;

	m_pVariableBlock->UsedListForwardLink = nNewFirstBlock;
	if (nNewFirstBlock == XQUEUE_INVALID_BLOCK)
	{
		m_pVariableBlock->UsedListForwardLink  = XQUEUE_INVALID_BLOCK;
		m_pVariableBlock->UsedListBackLink  = XQUEUE_INVALID_BLOCK;
	}
	else
	{
		_ASSERTE(nNewFirstBlock < m_nQueueSize);
		XQUEUE_DATA_BLOCK * pFirst = (XQUEUE_DATA_BLOCK *) (m_pDataBlock + nNewFirstBlock * m_nBlockSize);
		pFirst->BackLink = XQUEUE_INVALID_BLOCK;
	}
	m_pVariableBlock->UsedDataBlocks--;
	return XQUEUE_ERROR_SUCCESS;
}
// end  add by zhangwei 20100917

///////////////////////////////////////////////////////////////////////////////
//
// WriteToMMF() - private function
//
// Purpose:     Write memory-mapped file
//
// Parameters:  lpData    - [out] Pointer to the buffer that contains the
//                          data to write
//              nDataSize - [in] number of bytes of data in lpData
//
// Returns:     LONG      - XQueue error code (see XQueue.h)
//
// Notes:       Does not lock access to MMF.
//
// begin modify by zhangwei 20100917
//LONG CXQueue::WriteToMMF(LPBYTE lpData, size_t nDataSize)
//{
//	TRACE(_T("in CXQueue::WriteToMMF\n"));
//
//	// verify parameters
//	if ((lpData == NULL) ||
//		(nDataSize == 0) ||
//		IsBadReadPtr(lpData, nDataSize))
//	{
//		TRACE(_T("ERROR: bad parameters\n"));
//		_ASSERTE(FALSE);
//		return XQUEUE_ERROR_PARAMETER;
//	}
//
//	LONG lRet = XQUEUE_ERROR_SUCCESS;
//
//	size_t message_id = m_pVariableBlock->MessageId;
//	size_t nBlockNo = XQUEUE_INVALID_BLOCK;
//	XQUEUE_DATA_BLOCK * pPrevious = NULL;
//
//	while (nDataSize > 0)
//	{
//		XQUEUE_DATA_BLOCK * pXDB = GetFreeBlock();
//		if (!pXDB)
//		{
//			lRet = XQUEUE_ERROR_NO_FREE;
//			break;
//		}
//
//		AddUsed(pXDB);
//
//		size_t nBytesToCopy = (nDataSize > m_nDataSize) ?
//								m_nDataSize : nDataSize;
//
//		memcpy(pXDB->Data, lpData, nBytesToCopy);
//		pXDB->DataSize = nBytesToCopy;
//		pXDB->MessageId = message_id;
//
//		nBlockNo = pXDB->BlockNo;
//
//		pXDB->Dump();
//
//		lpData += nBytesToCopy;
//		nDataSize -= nBytesToCopy;
//
//		// if more bytes to copy, set continuation flag in this block
//		if (nDataSize > 0)
//			pXDB->DataSize |= XQUEUE_CONTINUATION_FLAG;
//
//		if (pPrevious)
//			pPrevious->ForwardLink = nBlockNo;
//
//		pPrevious = pXDB;
//	};
//
//	m_pVariableBlock->Dump();
//
//	return lRet;
//}
// end modify by zhangwei 20100917

// add by zhangwei 20100917
LONG CXQueue::WriteMultiBlock(LPBYTE lpData, size_t nDataSize, DWORD dwTimeOut, size_t nBlocks)
{
	LONG lRet = XQUEUE_ERROR_SUCCESS;
	XQUEUE_DATA_BLOCK * pWriteBlockLast = 0;
	XQUEUE_DATA_BLOCK * pWriteBlockFirst = 0;
	XQUEUE_DATA_BLOCK * pPrevious = 0;

	size_t nBlockNo = 0;
	size_t nCount = 0;
	clock_t ctStart = 0;
	clock_t ctEnd = 0;
	DWORD dwFreeTimeOut = dwTimeOut;
	
	for (size_t i = 0; i < nBlocks; i++)
	{
		if (dwFreeTimeOut != INFINITE)
			ctStart = clock();

		lRet = WaitForSemWrite(dwFreeTimeOut);
		if (lRet != XQUEUE_ERROR_SUCCESS)
			break;

		if (dwFreeTimeOut != INFINITE)
		{
			ctEnd = clock();
			DWORD dwDiff = (ctEnd - ctStart) * 1000 / CLOCKS_PER_SEC;
			if (dwFreeTimeOut >= dwDiff)
			{
				dwFreeTimeOut -= dwDiff;
			}
			else
			{
				dwFreeTimeOut = 0;
			}
		}

		lRet = WriteLock();
		if (lRet != XQUEUE_ERROR_SUCCESS)
			break;

		pWriteBlockLast = GetFreeBlock();
		if (!pWriteBlockLast)
		{
			WriteUnlock();
			lRet = XQUEUE_ERROR_NO_FREE;
			break;
		}
		WriteUnlock();

		nCount++;
		size_t nBytesToCopy = (nDataSize > m_nDataSize) ?
								m_nDataSize : nDataSize;

		memcpy(pWriteBlockLast->Data, lpData, nBytesToCopy);
		pWriteBlockLast->DataSize = nBytesToCopy;
		//pWriteBlockLast->MessageId = message_id;		

		pWriteBlockLast->Dump();

		lpData += nBytesToCopy;
		nDataSize -= nBytesToCopy;

		// if more bytes to copy, set continuation flag in this block
		if (nDataSize > 0)
			pWriteBlockLast->DataSize |= XQUEUE_CONTINUATION_FLAG;

		nBlockNo = pWriteBlockLast->BlockNo;
		if (pPrevious)
		{
			pPrevious->ForwardLink = nBlockNo;
			pWriteBlockLast->BackLink = pPrevious->BlockNo;
		}
		else
		{
			pWriteBlockLast->BackLink = XQUEUE_INVALID_BLOCK;
		}
		
		pWriteBlockLast->ForwardLink = XQUEUE_INVALID_BLOCK;
		pPrevious = pWriteBlockLast;

		if (0 == pWriteBlockFirst)
			pWriteBlockFirst = pWriteBlockLast;
	}

	if (lRet == XQUEUE_ERROR_SUCCESS)
	{
		ReadLock();
		AddUsedList(pWriteBlockFirst, pWriteBlockLast);
		m_pVariableBlock->UsedDataBlocks += nCount;
		ReadUnlock();
	}
	else if (nCount >= 1)
	{
		WriteLock();
		AddFreeList(pWriteBlockFirst, pWriteBlockLast);
		m_pVariableBlock->FreeDataBlocks += nCount;
		WriteUnlock();;
	}
	return lRet;
}

LONG CXQueue::WriteSingleBlock(LPBYTE lpData, size_t nDataSize, DWORD dwTimeOut)
{
	LONG lRet = XQUEUE_ERROR_SUCCESS;

	lRet = WaitForSemWrite(dwTimeOut);
	if (lRet != XQUEUE_ERROR_SUCCESS)
		return XQUEUE_ERROR_TIMEOUT;

	XQUEUE_DATA_BLOCK * pXDB = 0;

	lRet = WriteLock();
	if (lRet != XQUEUE_ERROR_SUCCESS)
		return XQUEUE_ERROR_TIMEOUT;

	pXDB = GetFreeBlock();
	if (!pXDB)
	{
		WriteUnlock();
		return XQUEUE_ERROR_NO_FREE;
	}
	WriteUnlock();

	memcpy(pXDB->Data, lpData, nDataSize);
	pXDB->DataSize = nDataSize;
	//pXDB->MessageId = message_id;

	pXDB->Dump();

	ReadLock();	
	AddUsed(pXDB);
	ReadUnlock();
	return XQUEUE_ERROR_SUCCESS;
}
//end add by zhangwei 20100917


///////////////////////////////////////////////////////////////////////////////
//
// ReleaseEntry() - private function
//
// Purpose:     Release all blocks in a queue entry, add them to free list.
//
// Parameters:  None
//
// Returns:     LONG - XQueue error code (see XQueue.h)
// begin modify by zhangwei 20100917 
// To used this function normaly,the read server must only one or must only one thread
//LONG CXQueue::ReleaseEntry()
//{
//	LONG lRet = XQUEUE_ERROR_SUCCESS;
//
//	XQUEUE_DATA_BLOCK *pXDB = NULL;
//
//	int nCount = 0;
//
//	for (;;)
//	{
//		// GetUsedBlock gets first block from front of list
//		pXDB = GetUsedBlock();
//		if (pXDB == NULL)
//			break;
//
//		nCount++;
//
//		size_t DataSize = pXDB->DataSize;
//
//		pXDB->DataSize = 0;
//
//		AddFree(pXDB);		// add to end of free list
//
//		// check if there is a continuation block
//		if ((DataSize & XQUEUE_CONTINUATION_FLAG) == 0)
//			break;
//	}
//
//	TRACE(_T("ReleaseEntry:  freed %d blocks, FreeDataBlocks=%d\n"),
//		nCount, m_pVariableBlock->FreeDataBlocks);
//
//	return lRet;
//}
// end modify by zhangwei 20100917 

///////////////////////////////////////////////////////////////////////////////
//
// Lock() - private function
//
// Purpose:     Wait for write mutex.
//
// Parameters:  dwTimeOut - [in] specifies the time-out interval, in
//                          milliseconds
//
// Returns:     LONG      - XQueue error code (see XQueue.h)
//
LONG CXQueue::WriteLock(DWORD dwTimeOut)
{
	_ASSERTE(m_hWriteMutex);
	if (!m_hWriteMutex)
		return XQUEUE_ERROR_INTERNAL;

	DWORD dwWaitResult = WaitForSingleObject(m_hWriteMutex, dwTimeOut);
	if (dwWaitResult == WAIT_OBJECT_0)
	{
		return XQUEUE_ERROR_SUCCESS;
	}
	else
	{
		TRACE(_T("TIMEOUT for thread %X -----\n"), GetCurrentThreadId());
		return XQUEUE_ERROR_TIMEOUT;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
// Unlock() - private function
//
// Purpose:     Unlock write mutex.
//
// Parameters:  None
//
// Returns:     LONG - XQueue error code (see XQueue.h)
//
LONG CXQueue::WriteUnlock()
{
	if (!m_hWriteMutex)
		return XQUEUE_ERROR_SUCCESS;

	ReleaseMutex(m_hWriteMutex);

	return XQUEUE_ERROR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
// add by zhangwei 20100917
// Lock() - private function
//
// Purpose:     Wait for write mutex.
//
// Parameters:  dwTimeOut - [in] specifies the time-out interval, in
//                          milliseconds
//
// Returns:     LONG      - XQueue error code (see XQueue.h)
//
LONG CXQueue::ReadLock(DWORD dwTimeOut)
{
	_ASSERTE(m_hReadMutex);
	if (!m_hReadMutex)
		return XQUEUE_ERROR_INTERNAL;

	DWORD dwWaitResult = WaitForSingleObject(m_hReadMutex, dwTimeOut);
	if (dwWaitResult == WAIT_OBJECT_0)
	{
		return XQUEUE_ERROR_SUCCESS;
	}
	else
	{
		TRACE(_T("TIMEOUT for thread %X -----\n"), GetCurrentThreadId());
		return XQUEUE_ERROR_TIMEOUT;
	}
}

///////////////////////////////////////////////////////////////////////////////
// add by zhangwei 20100917
// Unlock() - private function
//
// Purpose:     Unlock write mutex.
//
// Parameters:  None
//
// Returns:     LONG - XQueue error code (see XQueue.h)
//
LONG CXQueue::ReadUnlock()
{
	if (!m_hReadMutex)
		return XQUEUE_ERROR_SUCCESS;

	ReleaseMutex(m_hReadMutex);

	return XQUEUE_ERROR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//
// SetQueueAvailable() - private function
//
// Purpose:     Allow clients to write.
//
// Parameters:  None
//
// Returns:     LONG - XQueue error code (see XQueue.h)
//
//LONG CXQueue::SetQueueAvailable()
//{
//	_ASSERTE(m_hClientWriteEvent);
//
//	if (!m_hClientWriteEvent)
//		return XQUEUE_ERROR_NOT_OPEN;
//
//	// allow other clients to write
//	SetEvent(m_hClientWriteEvent);
//
//	return XQUEUE_ERROR_SUCCESS;
//}

///////////////////////////////////////////////////////////////////////////////
//
// WaitForQueueIdle() - private function
//
// Purpose:     Wait until no clients are writing.
//
// Parameters:  dwTimeOut - [in] specifies the time-out interval, in
//                          milliseconds
//
// Returns:     LONG - XQueue error code (see XQueue.h)
//
//LONG CXQueue::WaitForQueueIdle(DWORD dwTimeOut)
//{
//	_ASSERTE(m_hClientWriteEvent);
//
//	if (!m_hClientWriteEvent)
//		return XQUEUE_ERROR_NOT_OPEN;
//
//	DWORD dwWaitResult = WaitForSingleObject(m_hClientWriteEvent, dwTimeOut);
//	if (dwWaitResult == WAIT_OBJECT_0)
//		return XQUEUE_ERROR_SUCCESS;
//	else
//		return XQUEUE_ERROR_TIMEOUT;
//}

///////////////////////////////////////////////////////////////////////////////
// begin add by zhangwei 20100917
// WaitForQueueWrite() - private function
//
// Purpose:     Wait for semaphore to write
//
// Parameters:  dwTimeOut - [in] specifies the time-out interval, in
//                          milliseconds
//
// Returns:     LONG      - XQueue error code (see XQueue.h)
//
LONG CXQueue::WaitForSemWrite(DWORD dwTimeOut)
{
	_ASSERTE(m_hWriteSem);

	if (!m_hWriteSem)
		return XQUEUE_ERROR_NOT_OPEN;

	DWORD dwWaitResult = WaitForSingleObject(m_hWriteSem, dwTimeOut);
	if (dwWaitResult != WAIT_OBJECT_0)
		dwWaitResult = XQUEUE_ERROR_TIMEOUT;
	else
		dwWaitResult = XQUEUE_ERROR_SUCCESS;

	return dwWaitResult;
}

///////////////////////////////////////////////////////////////////////////////
// 
// WaitForSemRead() - private function
//
// Purpose:     Wait for semaphore to read
//
// Parameters:  dwTimeOut - [in] specifies the time-out interval, in
//                          milliseconds
//
// Returns:     LONG      - XQueue error code (see XQueue.h)
//
LONG CXQueue::WaitForSemRead(DWORD dwTimeOut)
{
	_ASSERTE(m_hReadSem);

	if (!m_hReadSem)
		return XQUEUE_ERROR_NOT_OPEN;

	DWORD dwWaitResult = WaitForSingleObject(m_hReadSem, dwTimeOut);
	if (dwWaitResult != WAIT_OBJECT_0)
		dwWaitResult = XQUEUE_ERROR_TIMEOUT;

	return dwWaitResult;
}
// end add by zhangwei 20100917

///////////////////////////////////////////////////////////////////////////////
// 
// WaitForQueueWrite() - private function
//
// Purpose:     Wait for event that signals something has been written to the
//              queue.
//
// Parameters:  dwTimeOut - [in] specifies the time-out interval, in
//                          milliseconds
//
// Returns:     LONG      - XQueue error code (see XQueue.h)
//
//LONG CXQueue::WaitForQueueWrite(DWORD dwTimeOut)
//{
//	_ASSERTE(m_hServerNotifyEvent);
//
//	if (!m_hServerNotifyEvent)
//		return XQUEUE_ERROR_NOT_OPEN;
//
//	DWORD dwWaitResult = WaitForSingleObject(m_hServerNotifyEvent, dwTimeOut);
//	if (dwWaitResult == WAIT_OBJECT_0)
//		return XQUEUE_ERROR_SUCCESS;
//	else
//		return XQUEUE_ERROR_TIMEOUT;
//}

///////////////////////////////////////////////////////////////////////////////
//
// CreateSyncObjects() - private function
//
// Purpose:     Create the mutex and event objects used by XQueue; called by
//              XQueue server.
//
// Parameters:  None
//
// Returns:     LONG - XQueue error code (see XQueue.h)
//
LONG CXQueue::CreateSyncObjects()
{
	TCHAR szName[_MAX_PATH*2] = { 0 };
	int len = 0;

	// first close any open handles

	if (m_hWriteMutex)
		::CloseHandle(m_hWriteMutex);
	m_hWriteMutex = NULL;

	//if (m_hClientWriteEvent)
	//	::CloseHandle(m_hClientWriteEvent);
	//m_hClientWriteEvent = NULL;

	//if (m_hServerNotifyEvent)
	//	::CloseHandle(m_hServerNotifyEvent);
	//m_hServerNotifyEvent = NULL;

	//add by zhangwei 20100917
	if (m_hWriteSem)
		::CloseHandle(m_hWriteSem);
	m_hWriteSem = NULL;

	if (m_hReadSem)
		::CloseHandle(m_hReadSem);
	m_hReadSem = NULL;

	if (m_hReadMutex)
		::CloseHandle(m_hReadMutex);
	m_hReadMutex = NULL;

	///////////////////////////////////////////////////////////////////////////
	// Create a mutex object. The server or client sets this to nonsignaled
	// when it is writing to the mmf.
	_tcscpy(szName, WRITE_MUTEX_NAME);
	len = _tcslen(szName);
	_tcsncat(szName, m_szQueueName, _countof(szName) - len - 1);

	m_hWriteMutex = CreateMutex(NULL,				// no security attributes
								FALSE,				// initially not owned
								szName);			// mutex name

	_ASSERTE(m_hWriteMutex);
	if (!m_hWriteMutex)
	{
		TRACE(_T("ERROR:  CreateMutex failed for m_hWriteMutex (%s)\n"),
			szName);
		return XQUEUE_ERROR_INTERNAL;
	}

	///////////////////////////////////////////////////////////////////////////
	// Create an auto reset event object. Clients set this to be signaled
	// after writing to the queue, to notify the server.
	//_tcscpy(szName, SERVER_NOTIFY_EVENT_NAME);
	//len = _tcslen(szName);
	//_tcsncat(szName, m_szQueueName, _countof(szName) - len - 1);

	//m_hServerNotifyEvent = CreateEvent(NULL,		// no security attributes
	//								   FALSE,		// auto reset event
	//								   FALSE,		// initial state is nonsignaled
	//								   szName);		// event name

	//_ASSERTE(m_hServerNotifyEvent);
	//if (!m_hServerNotifyEvent)
	//{
	//	TRACE(_T("ERROR:  CreateEvent failed for m_hServerNotifyEvent (%s)\n"),
	//		szName);
	//	::CloseHandle(m_hWriteMutex);
	//	m_hWriteMutex = NULL;
	//	return XQUEUE_ERROR_INTERNAL;
	//}

	/////////////////////////////////////////////////////////////////////////////
	//// Create an auto reset event object. Clients wait for this to be signaled
	//// before starting a new write.  This ensures that the server will free
	//// some blocks if there are insufficient free blocks.
	//_tcscpy(szName, CLIENT_WRITE_EVENT_NAME);
	//len = _tcslen(szName);
	//_tcsncat(szName, m_szQueueName, _countof(szName) - len - 1);

	//m_hClientWriteEvent = CreateEvent(NULL,			// no security attributes
	//								  FALSE,		// auto reset event
	//								  TRUE,			// initial state is signaled
	//								  szName);		// event name

	//_ASSERTE(m_hClientWriteEvent);
	//if (!m_hClientWriteEvent)
	//{
	//	TRACE(_T("ERROR:  CreateEvent failed for m_hClientWriteEvent (%s)\n"),
	//		szName);
	//	::CloseHandle(m_hWriteMutex);
	//	m_hWriteMutex = NULL;
	//	::CloseHandle(m_hServerNotifyEvent);
	//	m_hServerNotifyEvent = NULL;
	//	return XQUEUE_ERROR_INTERNAL;
	//}


	//add by zhangwei 20100917
	//
	_tcscpy(szName, WRITE_SEMAPHORE_NAME);
	len = _tcslen(szName);
	_tcsncat(szName, m_szQueueName, _countof(szName) - len - 1);

	m_hWriteSem = CreateSemaphore(NULL,				// no security attributes
								m_nQueueSize,		// initially  0
								m_nQueueSize,		// max count 
								szName);			// Semaphore name

	_ASSERTE(m_hWriteSem);
	if (!m_hWriteSem)
	{
		TRACE(_T("ERROR:  CreateSemaphore failed for m_hWriteSem (%s)\n"),
			szName);
		return XQUEUE_ERROR_INTERNAL;
	}

	//
	_tcscpy(szName, READ_SEMAPHORE_NAME);
	len = _tcslen(szName);
	_tcsncat(szName, m_szQueueName, _countof(szName) - len - 1);

	m_hReadSem = CreateSemaphore(NULL,				// no security attributes
								0,					// initially  0
								m_nQueueSize,		// max count 
								szName);			// Semaphore name

	_ASSERTE(m_hReadSem);
	if (!m_hReadSem)
	{
		TRACE(_T("ERROR:  CreateSemaphore failed for m_hReadSem (%s)\n"),
			szName);
		return XQUEUE_ERROR_INTERNAL;
	}

	//
	_tcscpy(szName, READ_MUTEX_NAME);
	len = _tcslen(szName);
	_tcsncat(szName, m_szQueueName, _countof(szName) - len - 1);

	m_hReadMutex = CreateMutex(NULL,				// no security attributes
								FALSE,				// initially not owned
								szName);			// mutex name

	_ASSERTE(m_hReadMutex);
	if (!m_hReadMutex)
	{
		TRACE(_T("ERROR:  CreateMutex failed for m_hWriteMutex (%s)\n"),
			szName);
		return XQUEUE_ERROR_INTERNAL;
	}
	return XQUEUE_ERROR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//
// OpenSyncObjects() - private function
//
// Purpose:     Open the mutex and event objects used by XQueue;  called by
//              XQueue clients to open objects created by server.
//
// Parameters:  None
//
// Returns:     LONG - XQueue error code (see XQueue.h)
//
LONG CXQueue::OpenSyncObjects()
{
	TCHAR szName[_MAX_PATH*2] = { 0 };
	int len = 0;

	// first close any open handles

	if (m_hWriteMutex)
		::CloseHandle(m_hWriteMutex);
	m_hWriteMutex = NULL;

	//if (m_hClientWriteEvent)
	//	::CloseHandle(m_hClientWriteEvent);
	//m_hClientWriteEvent = NULL;

	//if (m_hServerNotifyEvent)
	//	::CloseHandle(m_hServerNotifyEvent);
	//m_hServerNotifyEvent = NULL;

	//add by zhangwei 20100917
	if (m_hWriteSem)
		::CloseHandle(m_hWriteSem);
	m_hWriteSem = NULL;

	if (m_hReadSem)
		::CloseHandle(m_hReadSem);
	m_hReadSem = NULL;
	
	if (m_hReadMutex)
		::CloseHandle(m_hReadMutex);
	m_hReadMutex = NULL;

	///////////////////////////////////////////////////////////////////////////
	// Open the mutex object that was created by the server. The server or
	// client sets this to nonsignaled when it is writing to the mmf.
	_tcscpy(szName, WRITE_MUTEX_NAME);
	len = _tcslen(szName);
	_tcsncat(szName, m_szQueueName, _countof(szName) - len - 1);

	m_hWriteMutex = OpenMutex(MUTEX_ALL_ACCESS,		// all access
							  FALSE,				// not inheritable
							  szName);				// mutex name

	if (!m_hWriteMutex)
	{
		TRACE(_T("ERROR:  OpenMutex failed for m_hWriteMutex (%s)\n"),
			szName);
		return XQUEUE_ERROR_INTERNAL;
	}

	///////////////////////////////////////////////////////////////////////////
	// Open the event object that was created by the server.  The
	// client sets this to signaled when it writes to the queue.
	//_tcscpy(szName, SERVER_NOTIFY_EVENT_NAME);
	//len = _tcslen(szName);
	//_tcsncat(szName, m_szQueueName, _countof(szName) - len - 1);

	//m_hServerNotifyEvent = OpenEvent(EVENT_ALL_ACCESS,	// all access
	//								 FALSE,				// not inheritable
	//								 szName);			// event name

	//if (!m_hServerNotifyEvent)
	//{
	//	TRACE(_T("ERROR:  OpenEvent failed for m_hServerNotifyEvent (%s)\n"),
	//		szName);
	//	::CloseHandle(m_hWriteMutex);
	//	m_hWriteMutex = NULL;
	//	return XQUEUE_ERROR_INTERNAL;
	//}


	/////////////////////////////////////////////////////////////////////////////
	//// Open the auto reset event object. A client sets this to
	//// nonsignaled before attempting to write to the queue;
	//// other clients will check this event before attempting to write.
	//// This ensures that the server will free some blocks if there
	//// are insufficient free blocks.
	//_tcscpy(szName, CLIENT_WRITE_EVENT_NAME);
	//len = _tcslen(szName);
	//_tcsncat(szName, m_szQueueName, _countof(szName) - len - 1);

	//m_hClientWriteEvent = OpenEvent(EVENT_ALL_ACCESS,	// all access
	//							   FALSE,				// not inheritable
	//							   szName);				// event name

	//if (!m_hClientWriteEvent)
	//{
	//	TRACE(_T("ERROR:  OpenEvent failed for m_hClientWriteEvent (%s)\n"),
	//		szName);
	//	::CloseHandle(m_hWriteMutex);
	//	m_hWriteMutex = NULL;
	//	::CloseHandle(m_hServerNotifyEvent);
	//	m_hServerNotifyEvent = NULL;
	//	return XQUEUE_ERROR_INTERNAL;
	//}

	//add by zhangwei 20100917
	//
	_tcscpy(szName, WRITE_SEMAPHORE_NAME);
	len = _tcslen(szName);
	_tcsncat(szName, m_szQueueName, _countof(szName) - len - 1);

	m_hWriteSem = OpenSemaphore(SEMAPHORE_ALL_ACCESS ,		// all access
							  FALSE,				// not inheritable
							  szName);				// mutex name

	if (!m_hWriteSem)
	{
		TRACE(_T("ERROR:  OpenSemaphore failed for m_hWriteSem (%s)\n"),
			szName);
		return XQUEUE_ERROR_INTERNAL;
	}

	//
	_tcscpy(szName, READ_SEMAPHORE_NAME);
	len = _tcslen(szName);
	_tcsncat(szName, m_szQueueName, _countof(szName) - len - 1);

	m_hReadSem = OpenSemaphore(SEMAPHORE_ALL_ACCESS ,		// all access
							  FALSE,				// not inheritable
							  szName);				// mutex name

	if (!m_hReadSem)
	{
		TRACE(_T("ERROR:  OpenSemaphore failed for m_hReadSem (%s)\n"),
			szName);
		return XQUEUE_ERROR_INTERNAL;
	}

	//
	_tcscpy(szName, READ_MUTEX_NAME);
	len = _tcslen(szName);
	_tcsncat(szName, m_szQueueName, _countof(szName) - len - 1);

	m_hReadMutex = OpenMutex(MUTEX_ALL_ACCESS,		// no security attributes
								FALSE,				// initially not owned
								szName);			// mutex name

	_ASSERTE(m_hReadMutex);
	if (!m_hReadMutex)
	{
		TRACE(_T("ERROR:  CreateMutex failed for m_hWriteMutex (%s)\n"),
			szName);
		return XQUEUE_ERROR_INTERNAL;
	}
	return XQUEUE_ERROR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//
// CalculateNoBlocks() - private function
//
// Purpose:     Calculate number of blocks it will take for the specified
//              message size.
//
// Parameters:  nDataBytes - [in] number of bytes in message
//
// Returns:     size_t     - number of blocks required for message
//
size_t CXQueue::CalculateNoBlocks(size_t nDataBytes)
{
	TRACE(_T("in CXQueue::CalculateNoBlocks:  nDataBytes=%d  m_nDataSize=%d\n"),
		nDataBytes, m_nDataSize);

	size_t nBlocks = 0;

	nBlocks = nDataBytes / m_nDataSize;

	if ((nDataBytes % m_nDataSize) != 0)
		nBlocks++;

	if (nBlocks == 0)
		nBlocks = 1;

	TRACE(_T("CXQueue::CalculateNoBlocks returning %d =====\n"), nBlocks);

	return nBlocks;
}

// find the lost block and try remume
bool CXQueue::TryFindLost()
{
	if (0 == m_pVariableBlock)
		return false;

	long lRtn = ReadLock(1000);
	if (lRtn != XQUEUE_ERROR_SUCCESS)
		return false;

	//...
	ReadUnlock();
	return true;
}
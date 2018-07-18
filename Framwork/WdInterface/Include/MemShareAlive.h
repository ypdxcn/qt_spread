#pragma once
#include <map>
#include <string>
#include "osdepend.h"
#include "xmemmap.h"
#include "WdAp.h"

#define		MAX_NO_ALIVE	4

using namespace std;

#define MEMSHARE_INVALID_VALUE	0xFFFFFFFF
#define QUIT_CMD	0xabababab
#define START_CMD	0xbcbcbcbc
#define RESET_CMD	0xcdcdcdcd

typedef struct stAliveInf
{
	unsigned int uiAppNodeID;
	unsigned int uiAppAlive;
	unsigned int uiAppProcessID;
	unsigned int uiDogAlive;
	unsigned int uiDogProcessID;
	unsigned int uiAppQuitCmd;
	unsigned int uiAppIQuitNtf;
	unsigned int uiDogIQuitNtf;
	unsigned int uiOthQuitCmd;
	unsigned int uiOthQuitNodeID;

	stAliveInf()
	{
		uiAppNodeID = MEMSHARE_INVALID_VALUE;
		uiAppAlive = MEMSHARE_INVALID_VALUE;
		uiAppProcessID = MEMSHARE_INVALID_VALUE;
		uiDogAlive = MEMSHARE_INVALID_VALUE;
		uiDogProcessID = MEMSHARE_INVALID_VALUE;
		uiAppQuitCmd = MEMSHARE_INVALID_VALUE;
		uiAppIQuitNtf = MEMSHARE_INVALID_VALUE;
		uiDogIQuitNtf = MEMSHARE_INVALID_VALUE;
		uiOthQuitCmd = MEMSHARE_INVALID_VALUE;
		uiOthQuitNodeID = MEMSHARE_INVALID_VALUE;
	}
} ALIVE_INF;

typedef enum
{
	E_PROCESS_APP,
	E_PROCESS_DOG,
	E_PROCESS_UNKNOWN,
} E_PROC_TYPE;

typedef enum
{
	E_IS_ALIVE,
	E_NO_ALIVE,
	E_UNKNOWN_ALIVE
} ALIVE_STATUS;

//
class WDAP_CLASS CMemShareAlive :	public CXMemMapFile
{
public:
	CMemShareAlive(){}
	CMemShareAlive(E_PROC_TYPE eType);
	virtual ~CMemShareAlive(void);

	void Bind(E_PROC_TYPE eType) {m_uiProcessType = eType;}
	void SetNodeID(unsigned int uiNodeID);
	unsigned int AppNodeID();
	ALIVE_STATUS IsAlive();
	ALIVE_STATUS IsAlive(unsigned int& uiProcessID);
	void IamAlive();
	void IamAlive(unsigned int uiProcessID);

	void IQuitNtf();
	void YQuitCmd();
	void HQuitCmd(unsigned int uiCmd, unsigned int uiOthQuitNodeID);
	BOOL IsIQuitCmd();	
	BOOL IsHQuitCmd(unsigned int& uiOthQuitCmd, unsigned int& uiOthQuitNodeID);
	
public:
	BOOL Create(LPCTSTR lpszTblName, BOOL bReadOnly = FALSE);
	void Close();
	string ToString();
private:
	ALIVE_INF*		m_pAliveInf;
	unsigned int	m_uiNoAlive;
	unsigned int	m_uiProcessType;
private:

};

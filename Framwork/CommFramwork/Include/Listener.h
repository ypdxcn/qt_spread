#ifndef _LISTENER_H
#define _LISTENER_H

#include "Comm.h"
#include "WorkThreadNm.h"
#include "osdepend.h"

class CProtocolCommServer;
class COMM_CLASS CListener:virtual public CAction
{
	//friend class CProtocolCommServer;
public: 
	CListener(CProtocolCommServer *p);
	virtual ~CListener();

	virtual int Init(CConfig *	pConfig);
	virtual void Finish();
	virtual int Start();
	virtual void Stop();	
protected:
	virtual CAio* CreateAioListen() = 0;

	CProtocolCommServer * m_pCommServer;		//协议通信处理器(服务端)
	CAio *m_pAioListen;							//

	 //侦听IP和端口
	std::string m_sIp;
	int m_nPort;
	int m_nWaitConnNum;

	tsocket_t m_sListen;

public: 
	virtual int OnAccept(tsocket_t sSocket,const string& sIp,int nPort);
	virtual int OnAccept();
	tsocket_t SocketID(){return m_sListen;}
	int ListenPort(){return m_nPort;}
};
#endif
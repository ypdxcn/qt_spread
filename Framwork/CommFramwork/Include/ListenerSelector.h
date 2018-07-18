#ifndef _LISTENER_SELECTOR_H
#define _LISTENER_SELECTOR_H

#include "Selector.h"
#include "Listener.h"
#include "Comm.h"
#include "Singleton.h"



class CListenerSelector: public CListener 
{
	
public: 
	CListenerSelector(CProtocolCommServer *p):CListener(p){}
	~CListenerSelector(){}
private: 
	CAio* CreateAioListen(){return CSingleton<CSelectorListen>::Instance();}
};
#endif
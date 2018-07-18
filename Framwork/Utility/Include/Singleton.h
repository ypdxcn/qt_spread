#ifndef _SINGLETON_H
#define _SINGLETON_H
#include "Gess.h"

template<typename T>
class CSingleton 
{
public:
    static T* Instance()
	{
        if (0 == m_pInstance) 
		{
            m_mutexSingleton.Lock();
            if (0 == m_pInstance)
			{
                m_pInstance = new T();
                atexit(Destroy);
            }
			m_mutexSingleton.Unlock();
        }
        return m_pInstance;
    }
protected:
    CSingleton(){}
    ~CSingleton(){}
private:
	CSingleton(const CSingleton&); 
	CSingleton& operator=(const CSingleton&);
    static void Destroy()
	{
        if ( m_pInstance != 0 )
		{
            delete m_pInstance;
            m_pInstance = 0;
        }
    }
    static CGessMutex m_mutexSingleton;
    static T * volatile m_pInstance;
};
template<typename T>
CGessMutex CSingleton<T>::m_mutexSingleton;
template<typename T>
T * volatile CSingleton<T>::m_pInstance = 0;
#endif
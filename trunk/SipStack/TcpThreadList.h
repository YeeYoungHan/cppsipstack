
#ifndef _THREAD_LIST_H_
#define _THREAD_LIST_H_

#include "SipTcp.h"
#include "SipMutex.h"
#include <vector>
#include <string>

/**
 * @brief 쓰레드 리스트에 포함되는 하나의 쓰레드 정보 저장 클래스
 */
class CThreadListEntry
{
public:
	CThreadListEntry();

	Socket	m_hSend;				// 송신 pipe
	Socket	m_hRecv;				// 수신 pipe
	int			m_iSocketCount;	// 소켓 개수

	void IncreaseSocketCount( bool bLock = true );
	void DecreaseSocketCount( bool bLock = true );
};

typedef std::vector< CThreadListEntry * > THREAD_LIST;

/**
 * @brief 쓰레드 리스트 자료구조
 */
class CThreadList
{
public:
	CThreadList(void);
	~CThreadList(void);

#ifdef WIN32
	bool Init( int iThreadCount, int iThreadMaxCount, LPTHREAD_START_ROUTINE pThreadProc );
#else
	bool Init( int iThreadCount, int iThreadMaxCount, void *(*pThreadProc)(void*) );
#endif

	void Final();
	bool SetMaxSocketPerThread( int iMaxSocketPerThread );

	bool SendCommand( const char * pszData, int iDataLen, int iThreadIndex = -1, int * piThreadIndex = NULL );
	static int RecvCommand( Socket hSocket, char * pszData, int iDataSize );
	void GetString( std::string & strText );

private:
	THREAD_LIST	m_clsList;
	int					m_iMaxSocketPerThread;
	int					m_iThreadMaxCount;

#ifdef WIN32
	LPTHREAD_START_ROUTINE m_pThreadProc;
#else
	void *(*m_pThreadProc)(void*);
#endif

	bool AddThread();
	bool _SendCommand( Socket hSocket, const char * pszData, int iDataLen );
};

#endif

/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#include "SipStackDefine.h"
#include "TcpThreadList.h"
#include "MemoryDebug.h"

static CSipMutex gclsMutex;

CThreadListEntry::CThreadListEntry() : m_hSend(INVALID_SOCKET), m_hRecv(INVALID_SOCKET), m_iSocketCount(0), m_pUser(NULL)
{

}

/**
 * @ingroup SipStack
 * @brief 소켓을 종료한다.
 */
void CThreadListEntry::Close()
{
	if( m_hSend != INVALID_SOCKET )
	{
		closesocket( m_hSend );
		m_hSend = INVALID_SOCKET;
	}

	if( m_hRecv != INVALID_SOCKET )
	{
		closesocket( m_hRecv );
		m_hRecv = INVALID_SOCKET;
	}
}

/**
 * @ingroup SipStack
 * @brief 소켓 사용 개수를 증가시킨다.
 */
void CThreadListEntry::IncreaseSocketCount( bool bLock )
{
	if( bLock ) gclsMutex.acquire();
	++m_iSocketCount;
	if( bLock ) gclsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief 소켓 사용 개수를 감소시킨다.
 */
void CThreadListEntry::DecreaseSocketCount( bool bLock )
{
	if( bLock ) gclsMutex.acquire();
	--m_iSocketCount;
	if( bLock ) gclsMutex.release();
}

CThreadList::CThreadList() : m_iMaxSocketPerThread( 10 ), m_iThreadMaxCount(1), m_pThreadProc(NULL), m_pUser(NULL)
{
}

CThreadList::~CThreadList()
{
}

/**
 * @ingroup SipStack
 * @brief 쓰레드 리스트를 시작한다.
 * @param iThreadCount		생성할 쓰레드 개수
 * @param iThreadMaxCount	최대 쓰레드 개수
 * @param pThreadProc			쓰레드 함수
 * @param pUser						사용자 정의 변수
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
#ifdef WIN32
bool CThreadList::Init( int iThreadCount, int iThreadMaxCount, LPTHREAD_START_ROUTINE pThreadProc, void * pUser )
#else
bool CThreadList::Init( int iThreadCount, int iThreadMaxCount, void *(*pThreadProc)(void*), void * pUser )
#endif
{
	m_pThreadProc = pThreadProc;
	m_iThreadMaxCount = iThreadMaxCount;
	m_pUser = pUser;

	for( int i = 0; i < iThreadCount; ++i )
	{
		if( AddThread() == false )
		{
			return false;
		}
	}

	return true;
}

/**
 * @ingroup SipStack
 * @brief 쓰레드 리스트를 종료한다.
 */
void CThreadList::Final()
{
	THREAD_LIST::iterator	it;

	for( it = m_clsList.begin(); it != m_clsList.end(); ++it )
	{
		(*it)->Close();
		delete (*it);
	}

	m_clsList.clear();
}

/**
 * @ingroup SipStack
 * @brief 하나의 쓰레드에서 처리하는 최대 소켓 개수를 설정한다.
 * @param iMaxSocketPerThread 하나의 쓰레드에서 처리하는 최대 소켓 개수
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CThreadList::SetMaxSocketPerThread( int iMaxSocketPerThread )
{
	if( iMaxSocketPerThread <= 0 ) return false;

	m_iMaxSocketPerThread = iMaxSocketPerThread;

	return true;
}

/**
 * @ingroup SipStack
 * @brief 쓰레드에 명령을 전송한다.
 * @param pszData				명령
 * @param iDataLen			pszData 길이
 * @param iThreadIndex	명령을 전송할 쓰레드 인덱스
 *											-1 을 입력하면 최대 소켓 개수를 넘지 않은 쓰레드에 명령이 전송된다.
 * @param piThreadIndex	실제 전송된 쓰레드 인덱스를 저장하는 변수
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CThreadList::SendCommand( const char * pszData, int iDataLen, int iThreadIndex, int * piThreadIndex )
{
	bool	bRes = false, bFound = false;

	if( m_pThreadProc == NULL )
	{
		return false;
	}

	if( iThreadIndex < -1 )
	{
		return false;
	}

	if( iThreadIndex >= (int)m_clsList.size() )
	{
		return false;
	}

	gclsMutex.acquire();
	if( iThreadIndex == -1 )
	{
		THREAD_LIST::iterator	it;
		int iMinCount = 2000000000;

		// 소켓을 최소 사용하는 쓰레드를 검색한다.
		for( it = m_clsList.begin(), iThreadIndex = 0; it != m_clsList.end(); ++it, ++iThreadIndex )
		{
			if( iMinCount > (*it)->m_iSocketCount )
			{
				iMinCount = (*it)->m_iSocketCount;
				if( iMinCount == 0 ) break;
			}
		}

		if( iMinCount < m_iMaxSocketPerThread )
		{
			for( it = m_clsList.begin(), iThreadIndex = 0; it != m_clsList.end(); ++it, ++iThreadIndex )
			{
				if( iMinCount == (*it)->m_iSocketCount )
				{
					bRes = _SendCommand( (*it)->m_hSend, pszData, iDataLen );
					if( bRes ) (*it)->IncreaseSocketCount( false );
					if( piThreadIndex ) *piThreadIndex = iThreadIndex;
					bFound = true;
					break;
				}
			}
		}

		if( bFound == false )
		{
			if( AddThread() )
			{
				bRes = _SendCommand( m_clsList[m_clsList.size()-1]->m_hSend, pszData, iDataLen );
				if( bRes ) m_clsList[m_clsList.size()-1]->IncreaseSocketCount( false );
				if( piThreadIndex ) *piThreadIndex = iThreadIndex;
			}
		}
	}
	else
	{
		bRes = _SendCommand( m_clsList[iThreadIndex]->m_hSend, pszData, iDataLen );
		if( bRes ) m_clsList[m_clsList.size()-1]->IncreaseSocketCount( false );
		if( piThreadIndex ) *piThreadIndex = iThreadIndex;
	}
	gclsMutex.release();

	return bRes;
}

/**
 * @ingroup SipStack
 * @brief 명령을 수신한다.
 * @param hSocket		pipe 수신 핸들
 * @param pszData		명령 저장 변수
 * @param iDataSize pszData 변수 크기
 * @returns 수신한 길이를 리턴한다.
 */
int CThreadList::RecvCommand( Socket hSocket, char * pszData, int iDataSize )
{
	int	n;

#ifdef WIN32
	n = recv( hSocket, pszData, iDataSize, 0 );
#else
	n = read( hSocket, pszData, iDataSize );
#endif
	
	return n;
}

/**
 * @ingroup SipStack
 * @brief 쓰레드 정보를 하나의 문자열에 저장한다.
 * @param strBuf 쓰레드 정보를 저장할 문자열 변수
 */
void CThreadList::GetString( CMonitorString & strBuf )
{
	THREAD_LIST::iterator	it;

	strBuf.Clear();

	for( it = m_clsList.begin(); it != m_clsList.end(); ++it )
	{
		strBuf.AddRow( (*it)->m_iSocketCount );
	}
}

/**
 * @ingroup SipStack
 * @brief 쓰레드를 추가한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CThreadList::AddThread()
{
	if( (int)m_clsList.size() >= m_iThreadMaxCount )
	{
		return false;
	}

	Socket	arrSocket[2];
	CThreadListEntry * pclsThreadListEntry = new CThreadListEntry();
	if( pclsThreadListEntry == NULL )
	{
		return false;
	}

	if( pipe( arrSocket ) == -1 )
	{
		return false;
	}

	pclsThreadListEntry->m_hRecv = arrSocket[0];
	pclsThreadListEntry->m_hSend = arrSocket[1];
	pclsThreadListEntry->m_pUser = m_pUser;

	m_clsList.push_back( pclsThreadListEntry );

#ifdef WIN32
	DWORD		dwThreadId;
	HANDLE	hThread;

	hThread = CreateThread( NULL, 0, m_pThreadProc, pclsThreadListEntry, 0, &dwThreadId );
	if( hThread == NULL )
	{
		return false;
	}

	CloseHandle( hThread );
#else
	pthread_t	iThread;

	if( pthread_create( &iThread, NULL, m_pThreadProc, pclsThreadListEntry ) != 0 )
	{
		return false;
	}
	else
	{
		pthread_detach( iThread );
	}
#endif

	return true;
}

/**
 * @ingroup SipStack
 * @brief 쓰레드에 명령을 전송한다.
 * @param hSocket		pipe 전송 핸들
 * @param pszData		명령
 * @param iDataLen	pszData 길이
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CThreadList::_SendCommand( Socket hSocket, const char * pszData, int iDataLen )
{
	int	n;

#ifdef WIN32
	n = send( hSocket, pszData, iDataLen, 0 );
#else
	n = write( hSocket, pszData, iDataLen );
#endif
	if( n != iDataLen )
	{
		return false;
	}

	return true;
}

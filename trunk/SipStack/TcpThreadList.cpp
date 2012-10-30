
#include "SipStackDefine.h"
#include "TcpThreadList.h"

static CSipMutex gclsMutex;

CThreadListEntry::CThreadListEntry() : m_hSend(INVALID_SOCKET), m_hRecv(INVALID_SOCKET), m_iSocketCount(0)
{

}

/**
 * @brief ���� ��� ������ ������Ų��.
 */
void CThreadListEntry::IncreaseSocketCount( bool bLock )
{
	if( bLock ) gclsMutex.acquire();
	++m_iSocketCount;
	if( bLock ) gclsMutex.release();
}

/**
 * @brief ���� ��� ������ ���ҽ�Ų��.
 */
void CThreadListEntry::DecreaseSocketCount( bool bLock )
{
	if( bLock ) gclsMutex.acquire();
	--m_iSocketCount;
	if( bLock ) gclsMutex.release();
}

CThreadList::CThreadList(void) : m_iMaxSocketPerThread( 10 ), m_iThreadMaxCount(1)
{
}

CThreadList::~CThreadList(void)
{
}

/**
 * @brief ������ ����Ʈ�� �����Ѵ�.
 * @param iThreadCount	������ ������ ����
 * @param pThreadProc		������ �Լ�
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
#ifdef WIN32
bool CThreadList::Init( int iThreadCount, int iThreadMaxCount, LPTHREAD_START_ROUTINE pThreadProc )
#else
bool CThreadList::Init( int iThreadCount, int iThreadMaxCount, void *(*pThreadProc)(void*) )
#endif
{
	m_pThreadProc = pThreadProc;
	m_iThreadMaxCount = iThreadMaxCount;

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
 * @brief ������ ����Ʈ�� �����Ѵ�.
 */
void CThreadList::Final()
{
	THREAD_LIST::iterator	it;

	for( it = m_clsList.begin(); it != m_clsList.end(); ++it )
	{
		delete (*it);
	}
}

/**
 * @brief �ϳ��� �����忡�� ó���ϴ� �ִ� ���� ������ �����Ѵ�.
 * @param iMaxSocketPerThread �ϳ��� �����忡�� ó���ϴ� �ִ� ���� ����
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CThreadList::SetMaxSocketPerThread( int iMaxSocketPerThread )
{
	if( iMaxSocketPerThread <= 0 ) return false;

	m_iMaxSocketPerThread = iMaxSocketPerThread;

	return true;
}

/**
 * @brief �����忡 ����� �����Ѵ�.
 * @param pszData				���
 * @param iDataLen			pszData ����
 * @param iThreadIndex	����� ������ ������ �ε���
 *											-1 �� �Է��ϸ� �ִ� ���� ������ ���� ���� �����忡 ����� ���۵ȴ�.
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CThreadList::SendCommand( const char * pszData, int iDataLen, int iThreadIndex, int * piThreadIndex )
{
	bool	bRes = false, bFound = false;

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

		for( it = m_clsList.begin(), iThreadIndex = 0; it != m_clsList.end(); ++it, ++iThreadIndex )
		{
			if( m_iMaxSocketPerThread > (*it)->m_iSocketCount )
			{
				bRes = _SendCommand( (*it)->m_hSend, pszData, iDataLen );
				if( bRes ) (*it)->IncreaseSocketCount( false );
				if( piThreadIndex ) *piThreadIndex = iThreadIndex;
				bFound = true;
				break;
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
 * @brief ����� �����Ѵ�.
 * @param hSocket		pipe ���� �ڵ�
 * @param pszData		��� ���� ����
 * @param iDataSize pszData ���� ũ��
 * @returns ������ ���̸� �����Ѵ�.
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
 * @brief ������ ������ �ϳ��� ���ڿ��� �����Ѵ�.
 * @param strText ������ ������ ������ ���ڿ� ����
 */
void CThreadList::GetString( std::string & strText )
{
	THREAD_LIST::iterator	it;
	char	szText[11];

	strText.clear();

	for( it = m_clsList.begin(); it != m_clsList.end(); ++it )
	{
		snprintf( szText, sizeof(szText), "%d", (*it)->m_iSocketCount );
		strText.append( szText );
		strText.append( MR_ROW_SEP );
	}

	if( strText.empty() ) strText.append( MR_ROW_SEP );
}

/**
 * @brief �����带 �߰��Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief �����忡 ����� �����Ѵ�.
 * @param hSocket		pipe ���� �ڵ�
 * @param pszData		���
 * @param iDataLen	pszData ����
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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

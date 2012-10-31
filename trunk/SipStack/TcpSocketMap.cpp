
#include "SipStackDefine.h"
#include "TcpSocketMap.h"

CTcpSocketInfo::CTcpSocketInfo() : m_hSocket(INVALID_SOCKET)
{

}

CTcpSocketMap::CTcpSocketMap()
{
}

CTcpSocketMap::~CTcpSocketMap()
{
}

/**
 * @brief 
 * @param pszIp		Ŭ���̾�Ʈ IP �ּ�
 * @param iPort		Ŭ���̾�Ʈ ��Ʈ ��ȣ
 * @param hSocket TCP ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSocketMap::Insert( const char * pszIp, int iPort, Socket hSocket )
{
	std::string	strKey;
	CTcpSocketInfo	clsTcpSocketInfo;
	TCP_SOCKET_MAP::iterator	it;
	
	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	it = m_clsMap.find( strKey );
	if( it == m_clsMap.end() )
	{
		clsTcpSocketInfo.m_hSocket = hSocket;

		m_clsMap.insert( TCP_SOCKET_MAP::value_type( strKey, clsTcpSocketInfo ) );
	}
	m_clsMutex.release();

	return true;
}

/**
 * @brief Ŭ���̾�Ʈ IP �ּҿ� ��Ʈ ��ȣ�� ������ TCP ���� �ڵ��� �˻��Ѵ�.
 * @param pszIp		Ŭ���̾�Ʈ IP �ּ�
 * @param iPort		Ŭ���̾�Ʈ ��Ʈ ��ȣ
 * @param hSocket TCP ������ ������ ����
 * @returns �˻��� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpSocketMap::Select( const char * pszIp, int iPort, Socket & hSocket )
{
	bool	bRes = false;
	std::string	strKey;
	TCP_SOCKET_MAP::iterator	it;
	
	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	it = m_clsMap.find( strKey );
	if( it != m_clsMap.end() )
	{
		hSocket = it->second.m_hSocket;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @brief �ڷᱸ������ Ŭ���̾�Ʈ IP �ּҿ� ��Ʈ��ȣ�� �ش��ϴ� ������ �����Ѵ�.
 * @param pszIp Ŭ���̾�Ʈ IP �ּ�
 * @param iPort Ŭ���̾�Ʈ ��Ʈ ��ȣ
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSocketMap::Delete( const char * pszIp, int iPort )
{
	bool	bRes = false;
	std::string	strKey;
	TCP_SOCKET_MAP::iterator	it;
	
	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	it = m_clsMap.find( strKey );
	if( it != m_clsMap.end() )
	{
		m_clsMap.erase( it );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @brief TCP ���ǵ��� ������ �ϳ��� ���ڿ��� �����Ѵ�.
 * @param strText TCP ���ǵ��� ������ ������ ���ڿ� ����
 */
void CTcpSocketMap::GetString( std::string & strText )
{
	TCP_SOCKET_MAP::iterator	it;

	strText.clear();

	m_clsMutex.acquire();
	for( it = m_clsMap.begin(); it != m_clsMap.end(); ++it )
	{
		strText.append( it->first );
		strText.append( MR_ROW_SEP );
	}
	m_clsMutex.release();

	if( strText.empty() ) strText.append( MR_ROW_SEP );
}

/**
 * @brief Ŭ���̾�Ʈ IP �ּҿ� ��Ʈ ��ȣ�� �ڷᱸ���� Ű�� �����Ͽ� �����Ѵ�.
 * @param pszIp		Ŭ���̾�Ʈ IP �ּ�
 * @param iPort		Ŭ���̾�Ʈ ��Ʈ ��ȣ
 * @param strKey TCP/TLS ���� ������ �����ϴ� �ڷᱸ���� Ű ���� ����
 */
void CTcpSocketMap::GetKey( const char * pszIp, int iPort, std::string & strKey )
{
	char	szKey[51];

	snprintf( szKey, sizeof(szKey), "%s:%d", pszIp, iPort );
	strKey = szKey;
}

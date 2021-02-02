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

#include "SipPlatformDefine.h"
#include "TcpStack.h"
#include "TcpThread.h"
#include "ServerUtility.h"
#include "SipTcp.h"
#include "Log.h"
#include "MemoryDebug.h"

/**
 * @ingroup TcpStack
 * @brief TCP Ŭ���̾�Ʈ ������ ���� ����
 */
class CTcpClientArg
{
public:
	CTcpClientArg()
	{
	}

	~CTcpClientArg()
	{
	}

	/** TCP ���� IP �ּ� */
	std::string	m_strIp;

	/** TCP ���� ��Ʈ ��ȣ */
	int			m_iPort;

	/** TCP stack */
	CTcpStack * m_pclsStack;
};

/**
 * @ingroup TcpStack
 * @brief TCP ���� Thread
 * @param lpParameter CTcpClientArg ��ü
 * @returns 0 �� �����Ѵ�.
 */
THREAD_API TcpClientThread( LPVOID lpParameter )
{
	CTcpClientArg * pclsArg = (CTcpClientArg *)lpParameter;

	CLog::Print( LOG_INFO, "TcpClientThread started (%s:%d)", pclsArg->m_strIp.c_str(), pclsArg->m_iPort );

	Socket hConn = TcpConnect( pclsArg->m_strIp.c_str(), pclsArg->m_iPort, pclsArg->m_pclsStack->m_clsSetup.m_iTcpConnectTimeout );
	if( hConn == INVALID_SOCKET )
	{
		CLog::Print( LOG_ERROR, "%s TcpConnect(%s:%d) error(%d)", __FUNCTION__, pclsArg->m_strIp.c_str(), pclsArg->m_iPort, GetError() );

		pclsArg->m_pclsStack->m_clsClientMap.Delete( pclsArg->m_strIp.c_str(), pclsArg->m_iPort );
	}
	else
	{
		CTcpComm clsTcpComm;
		bool bAccept = true;

#ifdef USE_TLS
		if( pclsArg->m_pclsStack->m_clsSetup.m_bUseTls )
		{
			if( SSLConnect( hConn, &clsTcpComm.m_psttSsl ) == false )
			{
				CLog::Print( LOG_ERROR, "%s SSLConnect(%s:%d) error", __FUNCTION__, pclsArg->m_strIp.c_str(), pclsArg->m_iPort );
				closesocket( hConn );

				pclsArg->m_pclsStack->m_clsClientMap.Delete( pclsArg->m_strIp.c_str(), pclsArg->m_iPort );
				bAccept = false;
			}
		}
#endif

		if( bAccept )
		{
			if( pclsArg->m_pclsStack->m_clsSetup.m_bUseThreadPipe )
			{
				clsTcpComm.m_hSocket = hConn;
				snprintf( clsTcpComm.m_szIp, sizeof(clsTcpComm.m_szIp), "%s", pclsArg->m_strIp.c_str() );
				clsTcpComm.m_iPort = pclsArg->m_iPort;
				clsTcpComm.m_bClient = true;

				if( pclsArg->m_pclsStack->m_clsThreadList.SendCommand( (char *)&clsTcpComm, sizeof(clsTcpComm) ) == false )
				{
					CLog::Print( LOG_ERROR, "%s m_clsThreadList.SendCommand error", __FUNCTION__ );
					pclsArg->m_pclsStack->m_clsClientMap.Delete( pclsArg->m_strIp.c_str(), pclsArg->m_iPort );
					closesocket( hConn );
				}
			}
			else
			{
				CTcpNoPipeThreadArg * pclsNewArg = new CTcpNoPipeThreadArg();
				if( pclsNewArg == NULL )
				{
					CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
					closesocket( hConn );
				}
				else
				{
					pclsNewArg->m_hSocket = hConn;
					pclsNewArg->m_strIp = pclsArg->m_strIp;
					pclsNewArg->m_iPort = pclsArg->m_iPort;
					pclsNewArg->m_pclsStack = pclsArg->m_pclsStack;
					pclsNewArg->m_bClient = true;

					if( StartThread( "TcpNoPipeThread", TcpNoPipeThread, pclsNewArg ) == false )
					{
						CLog::Print( LOG_ERROR, "%s StartThread error", __FUNCTION__ );
						closesocket( hConn );
					}
				}

				pclsArg->m_pclsStack->m_clsClientMap.Delete( pclsArg->m_strIp.c_str(), pclsArg->m_iPort );
			}
		}
	}

	CLog::Print( LOG_INFO, "TcpClientThread terminated (%s:%d)", pclsArg->m_strIp.c_str(), pclsArg->m_iPort );

	delete pclsArg;

#ifdef USE_TLS
	if( pclsArg->m_pclsStack->m_clsSetup.m_bUseTls )
	{
#if OPENSSL_VERSION_NUMBER < 0x10100000L
		ERR_remove_thread_state( NULL );
#endif
	}
#endif
	
	return 0;
}

/**
 * @ingroup TcpStack
 * @brief TCP ���� Thread �� �����Ѵ�.
 * @param pszIp			TCP ���� IP �ּ�
 * @param iPort			TCP ���� ��Ʈ ��ȣ
 * @param pclsStack TCP stack
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool StartTcpClientThread( const char * pszIp, int iPort, CTcpStack * pclsStack )
{
	CTcpClientArg * pclsArg = new CTcpClientArg();
	if( pclsArg == NULL ) return false;

	pclsArg->m_strIp = pszIp;
	pclsArg->m_iPort = iPort;
	pclsArg->m_pclsStack = pclsStack;

	if( StartThread( "TcpClientThread", TcpClientThread, pclsArg ) == false )
	{
		delete pclsArg;
		return false;
	}

	return true;
}

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

#ifndef _SIP_STACK_H_
#define _SIP_STACK_H_

#include "SipStackDefine.h"
#include "SipTcp.h"
#include "SipStatusCode.h"
#include "SipICTList.h"
#include "SipNICTList.h"
#include "SipISTList.h"
#include "SipNISTList.h"
#include "SipStackSetup.h"
#include "SipStackCallBack.h"
#include "TcpThreadList.h"
#include "TcpSocketMap.h"
#include "TcpConnectMap.h"

typedef std::list< ISipStackCallBack * > SIP_STACK_CALLBACK_LIST;

/**
 * @defgroup SipStack SipStack
 * SIP Stack ���̺귯��
 */

/**
 * @ingroup SipStack
 * @brief SIP stack Ŭ����
 */
class CSipStack
{
public:
	CSipStack();
	~CSipStack();

	bool Start( CSipStackSetup & clsSetup );
	bool Stop( );

	bool Execute( struct timeval * psttTime );

	void IncreateUdpThreadCount( int & iThreadId );
	void DecreateUdpThreadCount();

	void IncreateTcpThreadCount( int & iThreadId );
	void DecreateTcpThreadCount();

	void GetString( CMonitorString & strBuf );
	void GetICTString( CMonitorString & strBuf );

	void Final();

	void DeleteAllTransaction();

	// SipStackCallBack.hpp
	bool AddCallBack( ISipStackCallBack * pclsCallBack );
	bool DeleteCallBack( ISipStackCallBack * pclsCallBack );
	void SetSecurityCallBack( ISipStackSecurityCallBack * pclsSecurityCallBack );

	void RecvRequest( int iThreadId, CSipMessage * pclsMessage );
	void RecvResponse( int iThreadId, CSipMessage * pclsMessage );
	void SendTimeout( int iThreadId, CSipMessage * pclsMessage );
	void TcpSessionEnd( const char * pszIp, int iPort, ESipTransport eTransport );
	void ThreadEnd( int iThreadId );

	// SipStackComm.hpp
	bool SendSipMessage( CSipMessage * pclsMessage );
	bool RecvSipMessage( int iThreadId, CSipMessage * pclsMessage );
	bool RecvSipMessage( int iThreadId, const char * pszBuf, int iBufLen, const char * pszIp, unsigned short iPort, ESipTransport eTransport );
	bool Send( CSipMessage * pclsMessage, bool bCheckMessage = true );
	bool Send( const char * pszMessage, const char * pszIp, unsigned short iPort, ESipTransport eTransport );

	bool	m_bStopEvent;						// SIP stack thread ���� �̺�Ʈ
	bool	m_bStackThreadRun;			// SIP stack thread �� ���� �߿� �ִ°�?

	Socket m_hUdpSocket;					// UDP SIP �޽��� ����/���� ���� �ڵ�
	Socket m_hTcpSocket;					// TCP SIP �޽����� ���� ���� ���� �ڵ�

	CSipMutex m_clsUdpRecvMutex;	// SIP �޽��� ���� ���ؽ�
	CSipStackSetup	m_clsSetup;		// SIP stack ����

	CThreadList			m_clsTcpThreadList;
	CTcpSocketMap		m_clsTcpSocketMap;
	CTcpConnectMap	m_clsTcpConnectMap;

#ifdef USE_TLS
	Socket m_hTlsSocket;					// TLS SIP �޽����� ���� ���� ���� �ڵ�

	CThreadList			m_clsTlsThreadList;
	CTcpSocketMap		m_clsTlsSocketMap;
	CTcpConnectMap	m_clsTlsConnectMap;
#endif

private:
	bool	 m_bStarted;
	int		 m_iUdpThreadRunCount;	// UDP ���� ������ ����
	int		 m_iTcpThreadRunCount;	// TCP ���� ������ ����

	CSipMutex m_clsMutex;
	CSipMutex m_clsUdpSendMutex;	// SIP �޽��� ���� ���ؽ�

	CSipICTList		m_clsICT;
	CSipNICTList	m_clsNICT;
	CSipISTList		m_clsIST;
	CSipNISTList	m_clsNIST;

	SIP_STACK_CALLBACK_LIST	m_clsCallBackList;
	ISipStackSecurityCallBack * m_pclsSecurityCallBack;

	bool _Stop( );
	int GetTcpConnectingCount( );
	void CheckSipMessage( CSipMessage * pclsMessage );
};

#endif

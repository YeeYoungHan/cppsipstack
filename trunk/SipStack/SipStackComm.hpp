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

/**
 * @ingroup SipStack
 * @brief SIP stack �� SIP �޽����� �������� ������ SIP stack �� SIP �޽����� �����ϰ� SIP �޽����� ��Ʈ��ũ�� �����Ѵ�.
 * @param pclsMessage SIP �޽��� ���� ����ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipStack::SendSipMessage( CSipMessage * pclsMessage )
{
	if( pclsMessage == NULL ) return false;

	CheckSipMessage( pclsMessage );
	++pclsMessage->m_iUseCount;

	if( pclsMessage->IsRequest() )
	{
		if( pclsMessage->IsMethod( "INVITE" ) || pclsMessage->IsMethod( "ACK" ) )
		{
			if( m_clsICT.Insert( pclsMessage ) )
			{
				Send( pclsMessage, false );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
		else
		{
			if( m_clsNICT.Insert( pclsMessage ) )
			{
				Send( pclsMessage, false );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
	}
	else
	{
		if( pclsMessage->IsMethod( "INVITE" ) )
		{
			if( m_clsIST.Insert( pclsMessage ) )
			{
				Send( pclsMessage, false );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
		else
		{
			if( m_clsNIST.Insert( pclsMessage ) )
			{
				Send( pclsMessage, false );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
	}

	delete pclsMessage;

	return false;
}

/**
 * @ingroup SipStack
 * @brief ��Ʈ��ũ���� ���ŵ� SIP �޽����� SIP stack �� �����ϰ� callback �޼ҵ带 ȣ���Ͽ� ���뿡 �˷� �ش�.
 *				�� �޼ҵ忡�� true �� �����ϸ� ���������� CSipMessage �޸𸮸� �����ϰ� �׷��� ������ ȣ���� �κп��� CSipMessage �޸𸮸� ������ �־�� �Ѵ�.
 * @param iThreadId		������ ���̵� ( 0 ���� ������ ���� )
 * @param pclsMessage SIP �޽��� ���� ����ü
 * @returns SIP stack �� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipStack::RecvSipMessage( int iThreadId, CSipMessage * pclsMessage )
{
	++pclsMessage->m_iUseCount;

	if( pclsMessage->IsRequest() )
	{
		if( pclsMessage->IsMethod( "INVITE" ) || pclsMessage->IsMethod( "ACK" ) )
		{
			if( m_clsIST.Insert( pclsMessage ) )
			{
				RecvRequest( iThreadId, pclsMessage );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
		else
		{
			if( m_clsNIST.Insert( pclsMessage ) )
			{
				RecvRequest( iThreadId, pclsMessage );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
	}
	else
	{
		if( pclsMessage->IsMethod( "INVITE" ) )
		{
			// INVITE �޽����� ���� CANCEL �޽����� �����ϸ� �̸� SIP stack ���� �����Ѵ�.
			if( pclsMessage->m_iStatusCode >= 200 )
			{
				m_clsNICT.DeleteCancel( pclsMessage );
			}

			if( m_clsICT.Insert( pclsMessage ) )
			{
				RecvResponse( iThreadId, pclsMessage );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
		else
		{
			if( m_clsNICT.Insert( pclsMessage ) )
			{
				RecvResponse( iThreadId, pclsMessage );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
	}

	delete pclsMessage;

	return false;
}

/**
 * @brief ��Ʈ��ũ���� ������ SIP �޽����� �Ľ��� ��, SIP stack �� �Է��Ѵ�.
 * @param iThreadId		������ ��ȣ
 * @param pszBuf			��Ʈ��ũ���� ���ŵ� SIP �޽���
 * @param iBufLen			��Ʈ��ũ���� ���ŵ� SIP �޽����� ����
 * @param pszIp				IP �ּ�
 * @param iPort				��Ʈ ��ȣ
 * @param eTransport	Transport
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipStack::RecvSipMessage( int iThreadId, const char * pszBuf, int iBufLen, const char * pszIp, unsigned short iPort, ESipTransport eTransport )
{
	CSipMessage	* pclsMessage = new CSipMessage();
	if( pclsMessage == NULL ) return false;

	if( pclsMessage->Parse( pszBuf, iBufLen ) == -1 )
	{
		delete pclsMessage;
		return false;
	}

	if( m_pclsSecurityCallBack )
	{
		const char * pszUserAgent = pclsMessage->m_strUserAgent.c_str();
		bool bDelete = false;

		if( m_pclsSecurityCallBack->IsDenyUserAgent( pszUserAgent ) ||
				m_pclsSecurityCallBack->IsAllowUserAgent( pszUserAgent ) == false ||
				m_pclsSecurityCallBack->IsDenyIp( pszIp ) ||
				m_pclsSecurityCallBack->IsAllowIp( pszIp ) == false )
		{
			bDelete = true;
		}

		if( bDelete )
		{
			delete pclsMessage;
			return false;
		}
	}

	if( pclsMessage->IsRequest() )
	{
		pclsMessage->AddIpPortToTopVia( pszIp, iPort );
	}

	pclsMessage->m_strClientIp = pszIp;
	pclsMessage->m_iClientPort = iPort;
	pclsMessage->m_eTransport = eTransport;

	RecvSipMessage( iThreadId, pclsMessage );

	return true;
}

/**
 * @ingroup SipStack
 * @brief SIP �޽����� ��Ʈ��ũ�� �����Ѵ�.
 * @param pclsMessage		SIP �޽��� ���� ����ü
 * @param bCheckMessage	SIP �޽����� �˻��Ͽ��� �ʼ� ����� �߰�/�����ϴ°�?
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipStack::Send( CSipMessage * pclsMessage, bool bCheckMessage )
{
	const char * pszIp = NULL;
	int iPort = -1;
	ESipTransport eTransport = E_SIP_UDP;

	if( bCheckMessage )
	{
		CheckSipMessage( pclsMessage );
	}

	if( pclsMessage->IsRequest() )
	{
		SIP_FROM_LIST::iterator itList = pclsMessage->m_clsRouteList.begin();
		if( itList == pclsMessage->m_clsRouteList.end() )
		{
			if( pclsMessage->m_clsReqUri.m_strHost.empty() ) return false;

			pszIp = pclsMessage->m_clsReqUri.m_strHost.c_str();
			iPort = pclsMessage->m_clsReqUri.m_iPort;
			eTransport = pclsMessage->m_clsReqUri.SelectTransport();
		}
		else
		{
			pszIp = itList->m_clsUri.m_strHost.c_str();
			iPort = itList->m_clsUri.m_iPort;
			eTransport = itList->m_clsUri.SelectTransport();
		}

		pclsMessage->m_eTransport = eTransport;
	}
	else
	{
		SIP_VIA_LIST::iterator itViaList = pclsMessage->m_clsViaList.begin();
		if( itViaList == pclsMessage->m_clsViaList.end() ) return false;

		const char * pszTemp;

		pszTemp = SearchSipParameter( itViaList->m_clsParamList, SIP_RPORT );
		if( pszTemp )
		{
			iPort = atoi( pszTemp );
		}
		else
		{
			iPort = itViaList->m_iPort;
		}

		pszIp = SearchSipParameter( itViaList->m_clsParamList, SIP_RECEIVED );
		if( pszIp == NULL )
		{
			pszIp = itViaList->m_strHost.c_str();
		}

		pszTemp = SearchSipParameter( itViaList->m_clsParamList, SIP_TRANSPORT );
		if( pszTemp )
		{
			if( !strcasecmp( pszTemp, SIP_TRANSPORT_TCP ) )
			{
				eTransport = E_SIP_TCP;
			}
			else if( !strcasecmp( pszTemp, SIP_TRANSPORT_TLS ) )
			{
				eTransport = E_SIP_TLS;
			}
		}
		else
		{
			const char * pszTransport = itViaList->m_strTransport.c_str();

			if( !strcasecmp( pszTransport, SIP_TRANSPORT_TCP ) )
			{
				eTransport = E_SIP_TCP;
			}
			else if( !strcasecmp( pszTransport, SIP_TRANSPORT_TLS ) )
			{
				eTransport = E_SIP_TLS;
			}
		}
	}

	if( iPort <= 0 ) iPort = 5060;

	if( pszIp[0] == '\0' ) return false;

	if( pclsMessage->m_strPacket.empty() )
	{
		if( pclsMessage->MakePacket() == false ) return false;
	}

	bool bRes = false;

	if( eTransport == E_SIP_UDP )
	{
		m_clsUdpSendMutex.acquire();
		bRes = UdpSend( m_hUdpSocket, pclsMessage->m_strPacket.c_str(), (int)pclsMessage->m_strPacket.length(), pszIp, iPort );
		m_clsUdpSendMutex.release();

		CLog::Print( LOG_NETWORK, "UdpSend(%s:%d) [%s]", pszIp, iPort, pclsMessage->m_strPacket.c_str() );
	}
	else if( eTransport == E_SIP_TCP )
	{
		Socket	hSocket;

		if( m_clsTcpSocketMap.Select( pszIp, iPort, hSocket ) )
		{
			SipTcpSend( hSocket, pszIp, iPort, pclsMessage );
		}
		else
		{
			bRes = StartSipTcpClientThread( this, pszIp, iPort, pclsMessage );
		}
	}
	else if( eTransport == E_SIP_TLS )
	{
#ifdef USE_TLS
		CTcpSocketInfo * pclsInfo = NULL;

		if( m_clsTlsSocketMap.Select( pszIp, iPort, &pclsInfo ) )
		{
			SipTlsSend( pclsInfo->m_hSocket, pclsInfo->m_psttSsl, pszIp, iPort, pclsMessage );
		}
		else
		{
			bRes = StartSipTlsClientThread( this, pszIp, iPort, pclsMessage );
		}
#else
		CLog::Print( LOG_ERROR, "TLS is not supported. rebuild with USE_TLS option" );
#endif
	}
	
	return bRes;
}

/**
 * @ingroup SipStack
 * @brief SIP �������� ���ڿ��� �����Ѵ�.
 * @param pszMessage ������ ���ڿ�
 * @param pszIp ������ IP �ּ�
 * @param iPort ������ ��Ʈ ��ȣ
 * @param eTransport ��������
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipStack::Send( const char * pszMessage, const char * pszIp, unsigned short iPort, ESipTransport eTransport )
{
	if( pszMessage == NULL || pszIp == NULL ) return false;

	bool bRes = false;
	int iMessageSize = (int)strlen( pszMessage );

	if( eTransport == E_SIP_UDP )
	{
		m_clsUdpSendMutex.acquire();
		bRes = UdpSend( m_hUdpSocket, pszMessage, iMessageSize, pszIp, iPort );
		m_clsUdpSendMutex.release();
	}
	else if( eTransport == E_SIP_TCP )
	{
		Socket	hSocket;

		if( m_clsTcpSocketMap.Select( pszIp, iPort, hSocket ) )
		{
			TcpSend( hSocket, pszMessage, iMessageSize );
		}
	}
	else if( eTransport == E_SIP_TLS )
	{
#ifdef USE_TLS
		CTcpSocketInfo * pclsInfo = NULL;

		if( m_clsTlsSocketMap.Select( pszIp, iPort, &pclsInfo ) )
		{
			SSLSend( pclsInfo->m_psttSsl, pszMessage, iMessageSize );
		}
#else
		CLog::Print( LOG_ERROR, "TLS is not supported. rebuild with USE_TLS option" );
#endif
	}
	
	return bRes;
}

/**
 * @ingroup SipStack
 * @brief ������ SIP �޽������� �ʿ��� ����� �������� ���� ��� default ����� �����Ѵ�.
 * @param pclsMessage ������ SIP �޽���
 */
void CSipStack::CheckSipMessage( CSipMessage * pclsMessage )
{
	if( pclsMessage->IsRequest() )
	{
		if( pclsMessage->m_clsViaList.size() == 0 )
		{
			pclsMessage->AddVia( m_clsSetup.m_strLocalIp.c_str(), m_clsSetup.GetLocalPort( pclsMessage->m_eTransport ) );
		}
	}

	if( pclsMessage->m_strSipVersion.empty() )
	{
		pclsMessage->m_strSipVersion = SIP_VERSION;
	}

	if( pclsMessage->m_clsContactList.size() == 0 )
	{
		ESipTransport eTransport = E_SIP_UDP;

		if( pclsMessage->IsRequest() )
		{
			SIP_FROM_LIST::iterator itList = pclsMessage->m_clsRouteList.begin();
			if( itList == pclsMessage->m_clsRouteList.end() )
			{
				eTransport = pclsMessage->m_clsReqUri.SelectTransport();
			}
			else
			{
				eTransport = itList->m_clsUri.SelectTransport();
			}
		}
		else
		{
			SIP_VIA_LIST::iterator itViaList = pclsMessage->m_clsViaList.begin();
			if( itViaList != pclsMessage->m_clsViaList.end() )
			{
				const char * pszTemp;

				pszTemp = SearchSipParameter( itViaList->m_clsParamList, SIP_TRANSPORT );
				if( pszTemp )
				{
					if( !strcasecmp( pszTemp, SIP_TRANSPORT_TCP ) )
					{
						eTransport = E_SIP_TCP;
					}
				}
				else
				{
					const char * pszTransport = itViaList->m_strTransport.c_str();

					if( !strcasecmp( pszTransport, SIP_TRANSPORT_TCP ) )
					{
						eTransport = E_SIP_TCP;
					}
				}
			}
		}

		CSipFrom clsContact;

		clsContact.m_clsUri.m_strProtocol = "sip";

		if( pclsMessage->IsRequest() )
		{
			clsContact.m_clsUri.m_strUser = pclsMessage->m_clsFrom.m_clsUri.m_strUser;
		}
		else
		{
			clsContact.m_clsUri.m_strUser = pclsMessage->m_clsTo.m_clsUri.m_strUser;
		}

		clsContact.m_clsUri.m_strHost = m_clsSetup.m_strLocalIp;
		clsContact.m_clsUri.m_iPort = m_clsSetup.m_iLocalUdpPort;
		clsContact.m_clsUri.InsertTransport( eTransport );

		pclsMessage->m_clsContactList.push_back( clsContact );
	}

	if( m_clsSetup.m_strUserAgent.empty() )
	{
		pclsMessage->m_strUserAgent = SIP_USER_AGENT;
	}
	else
	{
		pclsMessage->m_strUserAgent = m_clsSetup.m_strUserAgent;
	}

	if( pclsMessage->m_iMaxForwards == -1 )
	{
		pclsMessage->m_iMaxForwards = SIP_MAX_FORWARDS;
	}

	pclsMessage->m_bUseCompact = m_clsSetup.m_bUseSipCompactForm;
}

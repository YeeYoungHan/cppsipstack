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
 * @brief SIP stack 에 SIP 메시지가 존재하지 않으면 SIP stack 에 SIP 메시지를 저장하고 SIP 메시지를 네트워크로 전송한다.
 * @param pclsMessage SIP 메시지 저장 구조체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipStack::SendSipMessage( CSipMessage * pclsMessage )
{
	if( pclsMessage == NULL ) return false;

	if( m_clsSetup.m_bStateful )
	{
		CheckSipMessage( pclsMessage );
		++pclsMessage->m_iUseCount;

		if( pclsMessage->IsRequest() )
		{
			if( pclsMessage->IsMethod( SIP_METHOD_INVITE ) || pclsMessage->IsMethod( SIP_METHOD_ACK ) )
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
			if( pclsMessage->IsMethod( SIP_METHOD_INVITE ) )
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
	}
	else
	{
		Send( pclsMessage, false );

		if( pclsMessage->m_iUseCount == 0 )
		{
			delete pclsMessage;
		}

		return true;
	}

	return false;
}

/**
 * @ingroup SipStack
 * @brief 네트워크에서 수신된 SIP 메시지를 SIP stack 에 저장하고 callback 메소드를 호출하여 응용에 알려 준다.
 *				본 메소드에서 true 를 리턴하면 내부적으로 CSipMessage 메모리를 관리하고 그렇지 않으면 호출한 부분에서 CSipMessage 메모리를 삭제해 주어야 한다.
 * @param iThreadId		쓰레드 아이디 ( 0 부터 쓰레드 개수 )
 * @param pclsMessage SIP 메시지 저장 구조체
 * @returns SIP stack 에 저장하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipStack::RecvSipMessage( int iThreadId, CSipMessage * pclsMessage )
{
	if( m_clsSetup.m_bStateful )
	{
		++pclsMessage->m_iUseCount;

		if( pclsMessage->IsRequest() )
		{
			if( pclsMessage->IsMethod( SIP_METHOD_INVITE ) || pclsMessage->IsMethod( SIP_METHOD_ACK ) )
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
			if( pclsMessage->IsMethod( SIP_METHOD_INVITE ) )
			{
				// INVITE 메시지에 대한 CANCEL 메시지가 존재하면 이를 SIP stack 에서 삭제한다.
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
	}
	else
	{
		++pclsMessage->m_iUseCount;

		if( pclsMessage->IsRequest() )
		{
			RecvRequest( iThreadId, pclsMessage );
		}
		else
		{
			RecvResponse( iThreadId, pclsMessage );
		}

		--pclsMessage->m_iUseCount;
		if( pclsMessage->m_iUseCount == 0 )
		{
			delete pclsMessage;
		}

		return true;
	}

	return false;
}

/**
 * @brief 네트워크에서 수신한 SIP 메시지를 파싱한 후, SIP stack 에 입력한다.
 * @param iThreadId		쓰레드 번호
 * @param pszBuf			네트워크에서 수신된 SIP 메시지
 * @param iBufLen			네트워크에서 수신된 SIP 메시지의 길이
 * @param pszIp				IP 주소
 * @param iPort				포트 번호
 * @param eTransport	Transport
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
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
 * @brief SIP 메시지를 네트워크로 전송한다.
 * @param pclsMessage		SIP 메시지 저장 구조체
 * @param bCheckMessage	SIP 메시지를 검사하여서 필수 헤더를 추가/수정하는가?
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
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
			pszIp = (*itList)->m_clsUri.m_strHost.c_str();
			iPort = (*itList)->m_clsUri.m_iPort;
			eTransport = (*itList)->m_clsUri.SelectTransport();
		}

		pclsMessage->m_eTransport = eTransport;
	}
	else
	{
		SIP_VIA_LIST::iterator itViaList = pclsMessage->m_clsViaList.begin();
		if( itViaList == pclsMessage->m_clsViaList.end() ) return false;

		const char * pszTemp;

		pszTemp = (*itViaList)->SelectParamValue( SIP_RPORT );
		if( pszTemp )
		{
			iPort = atoi( pszTemp );
		}
		else
		{
			iPort = (*itViaList)->m_iPort;
		}

		pszIp = (*itViaList)->SelectParamValue( SIP_RECEIVED );
		if( pszIp == NULL )
		{
			pszIp = (*itViaList)->m_strHost.c_str();
		}

		pszTemp = (*itViaList)->SelectParamValue( SIP_TRANSPORT );
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
			const char * pszTransport = (*itViaList)->m_strTransport.c_str();

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
			SipTcpSend( hSocket, pszIp, iPort, pclsMessage, m_clsSetup.m_bUseContactListenPort ? m_clsSetup.m_iLocalTcpPort : 0 );
		}
		else
		{
			bRes = StartSipTcpClientThread( this, pszIp, iPort, pclsMessage );
		}
	}
	else if( eTransport == E_SIP_TLS )
	{
#ifdef USE_TLS
		if( m_clsTlsSocketMap.SendTls( pszIp, iPort, pclsMessage, m_clsSetup.m_bUseContactListenPort ? m_clsSetup.m_iLocalTlsPort : 0 ) )
		{
			bRes = true;
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
 * @brief SIP 세션으로 문자열을 전송한다.
 * @param pszMessage 전송할 문자열
 * @param pszIp 목적지 IP 주소
 * @param iPort 목적지 포트 번호
 * @param eTransport 프로토콜
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
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
			if( TcpSend( hSocket, pszMessage, iMessageSize ) == iMessageSize )
			{
				bRes = true;
			}
		}
	}
	else if( eTransport == E_SIP_TLS )
	{
#ifdef USE_TLS
		bRes = m_clsTlsSocketMap.SendTls( pszIp, iPort, pszMessage, iMessageSize );
#else
		CLog::Print( LOG_ERROR, "TLS is not supported. rebuild with USE_TLS option" );
#endif
	}
	
	return bRes;
}

/**
 * @ingroup SipStack
 * @brief 전송할 SIP 메시지에서 필요한 헤더가 존재하지 않을 경우 default 헤더를 저장한다.
 * @param pclsMessage 전송할 SIP 메시지
 */
void CSipStack::CheckSipMessage( CSipMessage * pclsMessage )
{
	if( pclsMessage->IsRequest() )
	{
		if( pclsMessage->m_clsViaList.size() == 0 )
		{
			int iPort = m_clsSetup.GetLocalPort( pclsMessage->m_eTransport );

			if( iPort == 0 ) iPort = 5060;
			pclsMessage->AddVia( m_clsSetup.m_strLocalIp.c_str(), iPort );
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
				eTransport = (*itList)->m_clsUri.SelectTransport();
			}
		}
		else
		{
			SIP_VIA_LIST::iterator itViaList = pclsMessage->m_clsViaList.begin();
			if( itViaList != pclsMessage->m_clsViaList.end() )
			{
				const char * pszTemp;

				pszTemp = (*itViaList)->SelectParamValue( SIP_TRANSPORT );
				if( pszTemp == NULL )
				{
					pszTemp = (*itViaList)->m_strTransport.c_str();
				}

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
			}
		}

		CSipFrom * pclsContact = new CSipFrom();
		if( pclsContact )
		{
			pclsContact->m_clsUri.m_strProtocol = SipGetProtocol( eTransport );

			if( pclsMessage->IsRequest() )
			{
				pclsContact->m_clsUri.m_strUser = pclsMessage->m_clsFrom.m_clsUri.m_strUser;
			}
			else
			{
				pclsContact->m_clsUri.m_strUser = pclsMessage->m_clsTo.m_clsUri.m_strUser;
			}

			pclsContact->m_clsUri.m_strHost = m_clsSetup.m_strLocalIp;
		
			if( eTransport == E_SIP_UDP )
			{
				pclsContact->m_clsUri.m_iPort = m_clsSetup.m_iLocalUdpPort;
			}
			else if( eTransport == E_SIP_TCP )
			{
				pclsContact->m_clsUri.m_iPort = m_clsSetup.m_iLocalTcpPort;
			}
			else if( eTransport == E_SIP_TLS )
			{
				pclsContact->m_clsUri.m_iPort = m_clsSetup.m_iLocalTlsPort;
			}

			pclsContact->m_clsUri.InsertTransport( eTransport );

			pclsMessage->m_clsContactList.push_back( pclsContact );
		}
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

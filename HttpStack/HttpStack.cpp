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

#include "HttpStack.h"
#include "HttpStatusCode.h"
#include "Log.h"
#include "Base64.h"
#include "MemoryDebug.h"

const EVP_MD * CHttpStack::m_psttMd = NULL;

CHttpStack::CHttpStack() : m_pclsCallBack(NULL)
{
	if( m_psttMd == NULL )
	{
		OpenSSL_add_all_digests();
		m_psttMd = EVP_get_digestbyname( "sha1" );
	}
}

CHttpStack::~CHttpStack()
{
}

/**
 * @ingroup HttpStack
 * @brief HTTP 서버를 시작한다.
 * @param pclsSetup			설정 객체
 * @param pclsCallBack	callback 객체
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttpStack::Start( CTcpStackSetup * pclsSetup, IHttpStackCallBack * pclsCallBack )
{
	if( pclsCallBack == NULL )
	{
		CLog::Print( LOG_ERROR, "%s IHttpStackCallBack is NULL", __FUNCTION__ );
		return false;
	}

	m_pclsCallBack = pclsCallBack;

	return m_clsTcpStack.Start( pclsSetup, this );
}

/**
 * @ingroup HttpStack
 * @brief 
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttpStack::Stop( )
{
	return m_clsTcpStack.Stop();
}

/**
 * @ingroup HttpStack
 * @brief WebSocket 클라이언트로 데이터를 전송한다.
 * @param pszClientIp WebSocket 클라이언트 IP 주소
 * @param iClientPort WebSocket 클라이언트 포트 번호
 * @param pszData			WebSocket 으로 전송할 payload 데이터
 * @param iDataLen		WebSocket 으로 전송할 payload 데이터 길이
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttpStack::SendWebSocketPacket( const char * pszClientIp, int iClientPort, const char * pszData, int iDataLen )
{
	int iPacketLen = 0;

	if( iDataLen > 65536 )
	{
		iPacketLen = 2 + 8 + iDataLen;
	}
	else if( iDataLen > 125 )
	{
		iPacketLen = 2 + 2 + iDataLen;
	}
	else
	{
		iPacketLen = 2 + iDataLen;
	}

	char * pszPacket = (char *)malloc( iPacketLen );
	if( pszPacket == NULL )
	{
		CLog::Print( LOG_ERROR, "%s malloc error", __FUNCTION__ );
		return false;
	}

	pszPacket[0] = (uint8_t)0x81;

	int iPayLoadPos = 2;

	if( iDataLen > 65536 )
	{
		pszPacket[1] = 127;

		uint64_t lDataLen = iDataLen;

		lDataLen = htonll( lDataLen );
		memcpy( pszPacket + 2, &lDataLen, 8 );
		iPayLoadPos = 2 + 8;
	}
	else if( iDataLen > 125 )
	{
		pszPacket[1] = 126;

		uint16_t sDataLen = iDataLen;

		sDataLen = htons( sDataLen );
		memcpy( pszPacket + 2, &sDataLen, 2 );
		iPayLoadPos = 2 + 2;
	}
	else
	{
		pszPacket[1] = iDataLen;
	}

	memcpy( pszPacket + iPayLoadPos, pszData, iDataLen );

	bool bRes = m_clsTcpStack.Send( pszClientIp, iClientPort, pszPacket, iPacketLen, false );

	free( pszPacket );

	return bRes;
}

/**
 * @ingroup HttpStack
 * @brief HTTP 클라이언트가 연결 이벤트 핸들러
 * @param pclsSessionInfo 세션 정보
 * @return HTTP 클라이언트 연결을 허용하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttpStack::InComingConnected( CTcpSessionInfo * pclsSessionInfo )
{
	return true;
}

/**
 * @ingroup HttpStack
 * @brief HTTP 클라이언트 세션이 종료 이벤트 핸들러
 * @param pclsSessionInfo 세션 정보
 */
void CHttpStack::SessionClosed( CTcpSessionInfo * pclsSessionInfo )
{
	if( pclsSessionInfo->m_pclsApp )
	{
		CHttpStackSession * pclsApp = (CHttpStackSession *)pclsSessionInfo->m_pclsApp;

		if( pclsApp->m_bWebSocket )
		{
			m_pclsCallBack->WebSocketClosed( pclsSessionInfo->m_strIp.c_str(), pclsSessionInfo->m_iPort );
		}
	}
}

/**
 * @ingroup HttpStack
 * @brief TCP 패킷 수신 이벤트 핸들러
 * @param pszPacket				수신 패킷
 * @param iPacketLen			수신 패킷 길이
 * @param pclsSessionInfo 세션 정보
 * @returns TCP 세션을 유지하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttpStack::RecvPacket( char * pszPacket, int iPacketLen, CTcpSessionInfo * pclsSessionInfo )
{
	if( pclsSessionInfo->m_pclsApp == NULL )
	{
		pclsSessionInfo->m_pclsApp = new CHttpStackSession();
		if( pclsSessionInfo->m_pclsApp == NULL )
		{
			CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
			return false;
		}
	}

	CHttpStackSession * pclsApp = (CHttpStackSession *)pclsSessionInfo->m_pclsApp;

	if( pclsApp->m_bWebSocket )
	{
		// WebSocket 프로토콜
		CWebSocketPacketHeader clsHeader;
		std::string strData;

		pclsApp->m_clsWsPacket.AddPacket( pszPacket, iPacketLen );

		while( pclsApp->m_clsWsPacket.GetData( clsHeader, strData ) )
		{
			if( clsHeader.m_iOpCode == 1 || clsHeader.m_iOpCode == 2 )
			{
				if( m_pclsCallBack->WebSocketData( pclsSessionInfo->m_strIp.c_str(), pclsSessionInfo->m_iPort, strData, pclsApp ) == false )
				{
					return false;
				}
			}
			else if( clsHeader.m_iOpCode == 8 )
			{
				if( pclsSessionInfo->Send( pszPacket, iPacketLen ) == false )
				{
					CLog::Print( LOG_ERROR, "%s Send error", __FUNCTION__ );
					return false;
				}
			}
			else if( clsHeader.m_iOpCode == 9 )
			{
				char szPacket[2];

				memset( szPacket, 0, sizeof(szPacket) );
				szPacket[0] = (uint8_t)0xFA;

				if( pclsSessionInfo->Send( szPacket, 2 ) == false )
				{
					CLog::Print( LOG_ERROR, "%s Send error", __FUNCTION__ );
					return false;
				}
			}
		}
	}
	else
	{
		// HTTP 프로토콜
		if( pclsApp->m_clsHttpPacket.AddPacket( pszPacket, iPacketLen ) == false )
		{
			CLog::Print( LOG_ERROR, "%s m_clsPacket.AddPacket error", __FUNCTION__ );
			return false;
		}

		if( pclsApp->m_clsHttpPacket.IsCompleted() )
		{
			CHttpMessage * pclsRecv = pclsApp->m_clsHttpPacket.GetHttpMessage();
			if( pclsRecv->IsRequest() )
			{
				CHttpMessage clsSend;
				CHttpHeader * pclsHeader;
				bool bClose = false;

				pclsHeader = pclsRecv->GetHeader( "User-Agent" );
				if( pclsHeader && strcmp( pclsApp->m_strUserAgent.c_str(), pclsHeader->m_strValue.c_str() ) )
				{
					pclsApp->m_strUserAgent = pclsHeader->m_strValue;
				}

				pclsHeader = pclsRecv->GetHeader( "Upgrade" );
				if( pclsHeader && !strcmp( pclsHeader->m_strValue.c_str(), "websocket" ) )
				{
					// WebSocket 을 위한 HTTP 요청이면 응용으로 callback 호출하지 않고 응답 메시지를 전송한다.
					if( MakeWebSocketResponse( pclsRecv, &clsSend ) == false )
					{
						return false;
					}

					pclsApp->m_bWebSocket = true;
				}
				else
				{
					pclsHeader = pclsRecv->GetHeader( "Connection" );
					if( pclsHeader == NULL || !strcmp( pclsHeader->m_strValue.c_str(), "close" ) )
					{
						bClose = true;
					}

					if( m_pclsCallBack->RecvHttpRequest( pclsRecv, &clsSend ) == false )
					{
						CLog::Print( LOG_ERROR, "%s RecvHttpRequest error", __FUNCTION__ );
						return false;
					}
				}

				int iNewBufLen = 8192 + clsSend.m_strBody.length();
				char * pszBuf = (char *)malloc( iNewBufLen );
				if( pszBuf == NULL )
				{
					CLog::Print( LOG_ERROR, "%s malloc error", __FUNCTION__ );
					return false;
				}
				
				int iBufLen = clsSend.ToString( pszBuf, iNewBufLen );
				if( iBufLen == -1 )
				{
					CLog::Print( LOG_ERROR, "%s ToString error", __FUNCTION__ );
					free( pszBuf );
					return false;
				}

				if( pclsSessionInfo->Send( pszBuf, iBufLen ) == false )
				{
					CLog::Print( LOG_ERROR, "%s Send error", __FUNCTION__ );
					free( pszBuf );
					return false;
				}

				free( pszBuf );

				if( bClose )
				{
					return false;
				}

				if( pclsApp->m_bWebSocket )
				{
					m_pclsCallBack->WebSocketConnected( pclsSessionInfo->m_strIp.c_str(), pclsSessionInfo->m_iPort );
				}
			}
			else
			{
				CLog::Print( LOG_ERROR, "%s http request error", __FUNCTION__ );
				return false;
			}
		}
	}

	return true;
}

/**
 * @ingroup HttpStack
 * @brief WebSocket 연결을 위한 HTTP 요청 메시지에 대한 HTTP 응답 메시지를 생성한다.
 * @param pclsRecv 수신된 HTTP 요청 메시지
 * @param pclsSend 전송할 HTTP 응답 메시지
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CHttpStack::MakeWebSocketResponse( CHttpMessage * pclsRecv, CHttpMessage * pclsSend )
{
	CHttpHeader * pclsHeader = pclsRecv->GetHeader( "Sec-WebSocket-Key" );
	if( pclsHeader == NULL )
	{
		CLog::Print( LOG_ERROR, "%s Sec-WebSocket-Key is not found", __FUNCTION__ );
		return false;
	}

	std::string strKey = pclsHeader->m_strValue;

	pclsSend->m_iStatusCode = HTTP_SWITCHING_PROTOCOLS;
	pclsSend->AddHeader( "Upgrade", "websocket" );
	pclsSend->AddHeader( "Connection", "Upgrade" );

	EVP_MD_CTX	sttCtx;
	uint8_t			szDigest[EVP_MAX_MD_SIZE];
	char				szOutput[EVP_MAX_MD_SIZE*2+1];
	uint32_t		iDigestLen;

	memset( szDigest, 0, sizeof(szDigest) );
	memset( szOutput, 0, sizeof(szOutput) );

	EVP_DigestInit( &sttCtx, m_psttMd );
	EVP_DigestUpdate( &sttCtx, strKey.c_str(), strKey.length() );
	EVP_DigestUpdate( &sttCtx, "258EAFA5-E914-47DA-95CA-C5AB0DC85B11", 36 );
	EVP_DigestFinal( &sttCtx, szDigest, &iDigestLen );

	Base64Encode( (char *)szDigest, iDigestLen, szOutput, sizeof(szOutput) );

	pclsSend->AddHeader( "Sec-WebSocket-Accept", szOutput );

	return true;
}

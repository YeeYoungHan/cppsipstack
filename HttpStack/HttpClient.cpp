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
#include "SipTcp.h"
#include "TlsFunction.h"
#include "HttpClient.h"
#include "Log.h"
#include "MemoryDebug.h"

CHttpClient::CHttpClient() : m_iRecvTimeout(10), m_iStatusCode(0)
{
	InitNetwork();
}

CHttpClient::~CHttpClient()
{
}

/**
 * @ingroup HttpStack
 * @brief	HTTP GET 명령을 실행한다.
 * @param pszUrl								HTTP URL (예:http://www.naver.com)
 * @param strOutputContentType	수신 Content-Type
 * @param strOutputBody					수신 body
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CHttpClient::DoGet( const char * pszUrl, std::string & strOutputContentType, std::string & strOutputBody )
{
	strOutputContentType.clear();
	strOutputBody.clear();

	if( pszUrl == NULL )
	{
		CLog::Print( LOG_ERROR, "%s pszUrl is null", __FUNCTION__ );
		return false;
	}

	CHttpUri clsUri;
	int iUrlLen = strlen( pszUrl );

	if( clsUri.Parse( pszUrl, iUrlLen ) == -1 )
	{
		CLog::Print( LOG_ERROR, "%s clsUri.Parse(%s) error", __FUNCTION__, pszUrl );
		return false;
	}

	CHttpMessage clsRequest;
	CHttpPacket	clsPacket;

	clsRequest.SetRequest( "GET", &clsUri );

	if( Execute( &clsUri, &clsRequest, &clsPacket ) )
	{
		CHttpMessage * pclsMessage = clsPacket.GetHttpMessage();

		strOutputContentType = pclsMessage->m_strContentType;
		strOutputBody = pclsMessage->m_strBody;
		return true;
	}

	return false;
}

/**
 * @ingroup HttpStack
 * @brief	HTTP GET 명령을 실행한다.
 * @param pszUrl								HTTP URL (예:http://wsf.cdyne.com/WeatherWS/Weather.asmx)
 * @param pszInputContentType		전송 Content-Type
 * @param pszInputBody					전송 body
 * @param strOutputContentType	수신 Content-Type
 * @param strOutputBody					수신 body
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CHttpClient::DoGet( const char * pszUrl, const char * pszInputContentType, const char * pszInputBody, std::string & strOutputContentType, std::string & strOutputBody )
{
	strOutputContentType.clear();
	strOutputBody.clear();

	if( pszUrl == NULL || pszInputContentType == NULL || pszInputBody == NULL )
	{
		CLog::Print( LOG_ERROR, "%s pszUrl or input content type or input body is null", __FUNCTION__ );
		return false;
	}

	CHttpUri clsUri;
	int iUrlLen = strlen( pszUrl );
	int iContentLength = strlen( pszInputBody );

	if( iContentLength <= 0 )
	{
		CLog::Print( LOG_ERROR, "%s pszInputBody's length(%d) error", __FUNCTION__, iContentLength );
		return false;
	}

	if( clsUri.Parse( pszUrl, iUrlLen ) == -1 )
	{
		CLog::Print( LOG_ERROR, "%s clsUri.Parse(%s) error", __FUNCTION__, pszUrl );
		return false;
	}

	CHttpMessage clsRequest;
	CHttpPacket clsPacket;

	clsRequest.SetRequest( "GET", &clsUri );
	clsRequest.m_strContentType = pszInputContentType;
	clsRequest.m_iContentLength = iContentLength;
	clsRequest.m_strBody = pszInputBody;

	if( Execute( &clsUri, &clsRequest, &clsPacket ) )
	{
		CHttpMessage * pclsMessage = clsPacket.GetHttpMessage();

		strOutputContentType = pclsMessage->m_strContentType;
		strOutputBody = pclsMessage->m_strBody;
		return true;
	}

	return false;
}

/**
 * @ingroup HttpStack
 * @brief	HTTP POST 명령을 실행한다.
 * @param pszUrl								HTTP URL (예:http://wsf.cdyne.com/WeatherWS/Weather.asmx)
 * @param pszInputContentType		전송 Content-Type
 * @param pszInputBody					전송 body
 * @param strOutputContentType	수신 Content-Type
 * @param strOutputBody					수신 body
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CHttpClient::DoPost( const char * pszUrl, const char * pszInputContentType, const char * pszInputBody, std::string & strOutputContentType, std::string & strOutputBody )
{
	return DoPost( pszUrl, NULL, pszInputContentType, pszInputBody, strOutputContentType, strOutputBody );
}

/**
 * @ingroup HttpStack
 * @brief	HTTP POST 명령을 실행한다.
 * @param pszUrl								HTTP URL (예:http://wsf.cdyne.com/WeatherWS/Weather.asmx)
 * @param pclsHeaderList				전송 헤더에 포함될 헤더 항목 리스트
 * @param pszInputContentType		전송 Content-Type
 * @param pszInputBody					전송 body
 * @param strOutputContentType	수신 Content-Type
 * @param strOutputBody					수신 body
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CHttpClient::DoPost( const char * pszUrl, HTTP_HEADER_LIST * pclsHeaderList, const char * pszInputContentType, const char * pszInputBody, std::string & strOutputContentType, std::string & strOutputBody )
{
	strOutputContentType.clear();
	strOutputBody.clear();

	if( pszUrl == NULL || pszInputContentType == NULL || pszInputBody == NULL )
	{
		CLog::Print( LOG_ERROR, "%s pszUrl or input content type or input body is null", __FUNCTION__ );
		return false;
	}

	CHttpUri clsUri;
	int iUrlLen = strlen( pszUrl );
	int iContentLength = strlen( pszInputBody );

	if( iContentLength <= 0 )
	{
		CLog::Print( LOG_ERROR, "%s pszInputBody's length(%d) error", __FUNCTION__, iContentLength );
		return false;
	}

	if( clsUri.Parse( pszUrl, iUrlLen ) == -1 )
	{
		CLog::Print( LOG_ERROR, "%s clsUri.Parse(%s) error", __FUNCTION__, pszUrl );
		return false;
	}

	CHttpMessage clsRequest;
	CHttpPacket clsPacket;

	clsRequest.SetRequest( "POST", &clsUri );
	clsRequest.m_strContentType = pszInputContentType;
	clsRequest.m_iContentLength = iContentLength;
	clsRequest.m_strBody = pszInputBody;

	if( pclsHeaderList )
	{
		clsRequest.m_clsHeaderList.insert( clsRequest.m_clsHeaderList.end(), pclsHeaderList->begin(), pclsHeaderList->end() );
	}

	if( Execute( &clsUri, &clsRequest, &clsPacket ) )
	{
		CHttpMessage * pclsMessage = clsPacket.GetHttpMessage();

		strOutputContentType = pclsMessage->m_strContentType;
		strOutputBody = pclsMessage->m_strBody;
		return true;
	}

	return false;
}

/**
 * @ingroup HttpClientApi
 * @brief HTTP POST 기반으로 SOAP 명령을 실행한다.
 * @param pszUrl				HTTP URL (예:http://wsf.cdyne.com/WeatherWS/Weather.asmx)
 * @param pszSoapAction HTTP SOAPAction 헤더에 저장될 문자열 (예:http://ws.cdyne.com/WeatherWS/GetWeatherInformation)
 * @param pszInputBody	전송 body
 * @param strOutputBody 수신 body
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CHttpClient::DoSoap( const char * pszUrl, const char * pszSoapAction, const char * pszInputBody, std::string & strOutputBody )
{
	return DoSoap( pszUrl, pszSoapAction, "text/xml;charset=UTF-8", pszInputBody, strOutputBody );
}

/**
 * @ingroup HttpClientApi
 * @brief HTTP POST 기반으로 SOAP 명령을 실행한다.
 * @param pszUrl				HTTP URL (예:http://wsf.cdyne.com/WeatherWS/Weather.asmx)
 * @param pszSoapAction HTTP SOAPAction 헤더에 저장될 문자열 (예:http://ws.cdyne.com/WeatherWS/GetWeatherInformation)
 * @param pszInputContentType	HTTP Content-Type
 * @param pszInputBody	전송 body
 * @param strOutputBody 수신 body
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CHttpClient::DoSoap( const char * pszUrl, const char * pszSoapAction, const char * pszInputContentType, const char * pszInputBody, std::string & strOutputBody )
{
	std::string strOutputContentType;

	strOutputBody.clear();

	if( pszSoapAction && strlen(pszSoapAction) > 0 )
	{
		HTTP_HEADER_LIST clsHeaderList;
		CHttpHeader clsHeader( "SOAPAction", pszSoapAction );

		clsHeaderList.push_back( clsHeader );

		return DoPost( pszUrl, &clsHeaderList, pszInputContentType, pszInputBody, strOutputContentType, strOutputBody );
	}
	
	return DoPost( pszUrl, NULL, pszInputContentType, pszInputBody, strOutputContentType, strOutputBody );
}

/**
 * @ingroup HttpStack
 * @brief HTTP 응답 메시지 수신 timeout 시간을 설정한다.
 * @param iRecvTimeout HTTP 응답 메시지 수신 timeout 시간 (초단위)
 */
void CHttpClient::SetRecvTimeout( int iRecvTimeout )
{
	m_iRecvTimeout = iRecvTimeout;
}

/**
 * @ingroup HttpStack
 * @brief HTTP 응답 status code 를 리턴한다.
 * @returns HTTP 응답 status code 를 리턴한다.
 */
int CHttpClient::GetStatusCode()
{
	return m_iStatusCode;
}

/**
 * @ingroup HttpStack
 * @brief HTTP 서버에 연결하여서 HTTP 요청 메시지를 전송한 후, HTTP 응답 메시지를 수신한다.
 * @param pclsUri				HTTP request URI
 * @param pclsRequest		HTTP request
 * @param pclsPacket		HTTP response 를 저장할 패킷 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CHttpClient::Execute( CHttpUri * pclsUri, CHttpMessage * pclsRequest, CHttpPacket * pclsPacket )
{
	char * pszBuf = NULL;
	int iBufLen, n;
	bool bRes = false;
	Socket hSocket = INVALID_SOCKET;
	SSL * psttSsl = NULL;
	CHttpMessage * pclsResponse = pclsPacket->GetHttpMessage();

	int iNewBufLen = 8192 + pclsRequest->m_strBody.length();
	pszBuf = (char *)malloc( iNewBufLen );
	if( pszBuf == NULL )
	{
		CLog::Print( LOG_ERROR, "%s malloc error", __FUNCTION__ );
		return false;
	}

	memset( pszBuf, 0, iNewBufLen );

	pclsRequest->m_strHttpVersion = "HTTP/1.1";
	
	iBufLen = pclsRequest->ToString( pszBuf, iNewBufLen );
	if( iBufLen <= 0 )
	{
		CLog::Print( LOG_ERROR, "%s clsRequest.ToString() error", __FUNCTION__ );
		goto FUNC_END;
	}

	hSocket = TcpConnect( pclsUri->m_strHost.c_str(), pclsUri->m_iPort );
	if( hSocket == INVALID_SOCKET )
	{
		CLog::Print( LOG_ERROR, "%s TcpConnect(%s:%d) error", __FUNCTION__, pclsUri->m_strHost.c_str(), pclsUri->m_iPort );
		goto FUNC_END;
	}

	CLog::Print( LOG_NETWORK, "TcpConnect(%s:%d) success", pclsUri->m_strHost.c_str(), pclsUri->m_iPort );

	// https 프로토콜이면 TLS 로 연결한다.
	if( !strcmp( pclsUri->m_strProtocol.c_str(), "https" ) )
	{
		if( SSLConnect( hSocket, &psttSsl ) == false )
		{
			CLog::Print( LOG_ERROR, "%s SSLConnect error", __FUNCTION__ );
			goto FUNC_END;
		}

		CLog::Print( LOG_NETWORK, "SSLConnect(%s:%d) success", pclsUri->m_strHost.c_str(), pclsUri->m_iPort );
	}

	if( psttSsl )
	{
		if( SSLSend( psttSsl, pszBuf, iBufLen ) != iBufLen )
		{
			CLog::Print( LOG_ERROR, "%s SSLSend error", __FUNCTION__ );
			goto FUNC_END;
		}
	}
	else
	{
		if( TcpSend( hSocket, pszBuf, iBufLen ) != iBufLen )
		{
			CLog::Print( LOG_ERROR, "%s TcpSend error", __FUNCTION__ );
			goto FUNC_END;
		}
	}

	CLog::Print( LOG_NETWORK, "TcpSend(%s:%d) [%s]", pclsUri->m_strHost.c_str(), pclsUri->m_iPort, pszBuf );

	while( 1 )
	{
		memset( pszBuf, 0, iNewBufLen );

		if( psttSsl )
		{
			n = SSLRecv( psttSsl, pszBuf, iNewBufLen );
		}
		else
		{
			n = TcpRecv( hSocket, pszBuf, iNewBufLen, m_iRecvTimeout );
		}

		if( n <= 0 )
		{
			break;
		}

		CLog::Print( LOG_NETWORK, "TcpRecv(%s:%d) [%.*s]", pclsUri->m_strHost.c_str(), pclsUri->m_iPort, n, pszBuf );

		if( pclsPacket->AddPacket( pszBuf, n ) == false )
		{
			CLog::Print( LOG_ERROR, "%s clsPacket.AddPacket error", __FUNCTION__ );
			break;
		}

		if( pclsPacket->IsCompleted() ) break;
	}

	m_iStatusCode = pclsResponse->m_iStatusCode;

	if( pclsResponse->m_iStatusCode / 100 == 2 )
	{
		bRes = true;
	}

FUNC_END:
	if( psttSsl )
	{
		SSLClose( psttSsl );
		psttSsl = NULL;
	}

	if( hSocket != INVALID_SOCKET )
	{
		closesocket( hSocket );
		hSocket = INVALID_SOCKET;
	}

	if( pszBuf )
	{
		free( pszBuf );
		pszBuf = NULL;
	}

	// 3XX 응답에서 Location 헤더가 존재하면 해당 URL 로 다시 시도한다.
	if( pclsResponse->m_iStatusCode / 100 == 3 )
	{
		CHttpHeader * pclsHeader = pclsResponse->GetHeader( "Location" );
		if( pclsHeader && pclsHeader->m_strValue.empty() == false )
		{
			CHttpUri clsUri;

			if( clsUri.Parse( pclsHeader->m_strValue.c_str(), pclsHeader->m_strValue.length() ) )
			{
				if( clsUri.m_strHost.empty() )
				{
					clsUri.m_strHost = pclsUri->m_strHost;
					clsUri.m_iPort = pclsUri->m_iPort;
					clsUri.m_strProtocol = pclsUri->m_strProtocol;
					clsUri.m_strPath = pclsHeader->m_strValue;
				}
				
				pclsRequest->SetRequest( pclsRequest->m_strHttpMethod.c_str(), &clsUri );

				return Execute( &clsUri, pclsRequest, pclsPacket );
			}
		}
	}

	return bRes;
}

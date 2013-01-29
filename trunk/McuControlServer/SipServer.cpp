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

#include "SipServer.h"
#include "SipUtility.h"
#include "UserMap.h"
#include "McuControlServer.h"
#include "CallMap.h"
#include "StringUtility.h"

CSipStack	gclsSipStack;
CSipServer gclsSipServer;

#include "SipServerRegister.hpp"
#include "SipServerUtility.hpp"

CSipServer::CSipServer()
{
}

CSipServer::~CSipServer()
{
}

/**
 * @ingroup McuControlServer
 * @brief SIP 서버를 시작한다.
 * @param clsSetup SIP stack 설정 항목을 저장한 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipServer::Start( CSipStackSetup & clsSetup )
{
	gclsSipStack.AddCallBack( this );

	if( gclsSipStack.Start( clsSetup ) == false ) return false;

	return true;
}

/**
 * @ingroup McuControlServer
 * @brief SIP 요청 메시지 수신 이벤트 핸들러
 * @param iThreadId		쓰레드 아이디
 * @param pclsMessage SIP 요청 메시지
 * @returns SIP 요청 메시지를 처리하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServer::RecvRequest( int iThreadId, CSipMessage * pclsMessage )
{
	if( pclsMessage->IsMethod( "REGISTER" ) )
	{
		return RecvRegisterRequest( iThreadId, pclsMessage );
	}

	std::string strToId = pclsMessage->m_clsTo.m_clsUri.m_strUser;

	if( strToId.length() == 0 )
	{
		strToId = pclsMessage->m_clsTo.m_strDisplayName;
	}

	CUserInfo clsUserInfo;

	if( !strncmp( strToId.c_str(), MCU_CONFERENCE_PREFIX, strlen(MCU_CONFERENCE_PREFIX) ) )
	{
		// 회의룸 입장 요청
		clsUserInfo.m_strIp = MCU_IP;
		clsUserInfo.m_iPort = MCU_PORT;
	}
	else
	{
		if( gclsUserMap.Select( strToId.c_str(), clsUserInfo ) == false )
		{
			// TO 사용자가 존재하지 않으면 404 NOT FOUND 로 응답한다.
			gclsSipStack.SendSipMessage( pclsMessage->CreateResponseWithToTag( SIP_NOT_FOUND ) );
			return true;
		}

		if( strcmp( pclsMessage->m_strClientIp.c_str(), MCU_IP ) )
		{
			if( pclsMessage->IsMethod( "INVITE" ) )
			{
				// 영상 디바이스에서 수신되었고 1:1 통화 INVITE 요청 메시지이면 통화 자료구조에 저장한다.
				if( gclsCallMap.Insert( pclsMessage ) == false )
				{
					gclsSipStack.SendSipMessage( pclsMessage->CreateResponseWithToTag( SIP_INTERNAL_SERVER_ERROR ) );
					return true;
				}
			}

			gclsCallMap.Update( pclsMessage );

			clsUserInfo.m_strIp = MCU_IP;
			clsUserInfo.m_iPort = MCU_PORT;
		}
	}

	// TO 사용자로 SIP 요청 메시지를 전달한다.
	SIP_VIA_LIST::iterator itVia = pclsMessage->m_clsViaList.begin();
	if( itVia != pclsMessage->m_clsViaList.end() )
	{
		const char * pszBranch = itVia->GetParamValue( "branch" );
		std::string strBranch;

		if( pszBranch )
		{
			strBranch = pszBranch;
			strBranch.append( "_copy" );
		}

		CSipMessage * pclsRequest = new CSipMessage();
		if( pclsRequest )
		{
			*pclsRequest = *pclsMessage;
			
			pclsRequest->AddVia( gclsSipStack.m_clsSetup.m_strLocalIp.c_str(), gclsSipStack.m_clsSetup.m_iLocalUdpPort, strBranch.c_str() );
			pclsRequest->AddRoute( clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort );

			SIP_FROM_LIST::iterator itContact = pclsRequest->m_clsContactList.begin();
			if( itContact != pclsRequest->m_clsContactList.end() )
			{
				itContact->m_clsUri.m_strHost = gclsSipStack.m_clsSetup.m_strLocalIp;
				itContact->m_clsUri.m_iPort = gclsSipStack.m_clsSetup.m_iLocalUdpPort;
			}

			UpdateBranch( pclsRequest, strBranch );

			gclsSipStack.SendSipMessage( pclsRequest );
			return true;
		}
	}

	return false;
}

/**
 * @ingroup McuControlServer
 * @brief SIP 응답 메시지 수신 이벤트 핸들러
 * @param iThreadId		쓰레드 아이디
 * @param pclsMessage SIP 응답 메시지
 * @returns SIP 응답 메시지를 처리하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServer::RecvResponse( int iThreadId, CSipMessage * pclsMessage )
{
	CSipMessage * pclsResponse = new CSipMessage();
	if( pclsResponse )
	{
		// SIP 요청 메시지를 전송한 호스트로 전달한다.
		*pclsResponse = *pclsMessage;
		pclsResponse->m_clsViaList.pop_front();

		gclsCallMap.Update( pclsMessage );

		if( pclsResponse->m_clsViaList.size() > 0 )
		{
			SIP_FROM_LIST::iterator itContact = pclsResponse->m_clsContactList.begin();
			if( itContact != pclsResponse->m_clsContactList.end() )
			{
				itContact->m_clsUri.m_strHost = gclsSipStack.m_clsSetup.m_strLocalIp;
				itContact->m_clsUri.m_iPort = gclsSipStack.m_clsSetup.m_iLocalUdpPort;
			}
			
			gclsSipStack.SendSipMessage( pclsResponse );
		}

		if( !strcmp( pclsMessage->m_strClientIp.c_str(), MCU_IP ) )
		{
			if( pclsMessage->IsMethod( "INVITE" ) && pclsMessage->m_iStatusCode == SIP_OK )
			{
				CCallInfo clsCallInfo;

				if( gclsCallMap.Select( pclsMessage, clsCallInfo ) )
				{
					if( clsCallInfo.m_bRecvFromDevice )
					{
						// 1:1 통화 요청을 수신자에게 전달한다.
						CSipMessage * pclsRequest = new CSipMessage();
						*pclsRequest = clsCallInfo.m_clsSipMessage;

						pclsRequest->m_clsViaList.clear();
						pclsRequest->m_clsCallId.m_strName.append( "_copy" );

						CUserInfo	clsUserInfo;

						std::string strToId = pclsMessage->m_clsTo.m_clsUri.m_strUser;
						if( strToId.length() == 0 )
						{
							strToId = pclsMessage->m_clsTo.m_strDisplayName;
						}

						pclsRequest->m_clsReqUri.m_strUser = strToId;
						pclsRequest->m_clsReqUri.m_strHost = gclsSipStack.m_clsSetup.m_strLocalIp;
						pclsRequest->m_strSipMethod = "INVITE";
						pclsRequest->m_iStatusCode = -1;

						/* 아래의 코드를 추가하면 영상 디바이스에서 MCU 간의 SIP INFO 메시지 등을 정상적으로 relay 시켜주어야 한다.
								But 현재는 From, To tag 를 관리하고 있지 않으므로 정상적인 relay 는 안 되고 있다.
 						SIP_FROM_LIST::iterator itContact = pclsRequest->m_clsContactList.begin();
						if( itContact != pclsRequest->m_clsContactList.end() )
						{
							itContact->m_clsUri.m_strHost = gclsSipStack.m_clsSetup.m_strLocalIp;
							itContact->m_clsUri.m_iPort = gclsSipStack.m_clsSetup.m_iLocalUdpPort;
						}
						*/

						if( gclsUserMap.Select( strToId.c_str(), clsUserInfo ) )
						{
							pclsRequest->AddRoute( clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort );
							gclsCallMap.Insert( pclsRequest, clsCallInfo.m_strRoomId );
							gclsSipStack.SendSipMessage( pclsRequest );
						}
					}
					else
					{
						// Re-INVITE
						CSipMessage * pclsRequest = new CSipMessage();
						*pclsRequest = clsCallInfo.m_clsSipMessage;
						pclsRequest->m_clsViaList.clear();

						CUserInfo	clsUserInfo;

						std::string strFromId = pclsMessage->m_clsFrom.m_clsUri.m_strUser;
						if( strFromId.length() == 0 )
						{
							strFromId = pclsMessage->m_clsFrom.m_strDisplayName;
						}

						pclsRequest->m_clsReqUri.m_strUser = strFromId;
						pclsRequest->m_clsReqUri.m_strHost = gclsSipStack.m_clsSetup.m_strLocalIp;
						pclsRequest->m_strSipMethod = "INVITE";
						pclsRequest->m_iStatusCode = -1;
						pclsRequest->m_clsCSeq.m_iDigit = pclsMessage->m_clsCSeq.m_iDigit;

						if( gclsUserMap.Select( strFromId.c_str(), clsUserInfo ) )
						{
							pclsRequest->m_clsRouteList.clear();
							pclsRequest->AddRoute( clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort );
							gclsSipStack.SendSipMessage( pclsRequest );
						}

						// ACK 전송한다.
						pclsRequest = new CSipMessage();
						*pclsRequest = clsCallInfo.m_clsSipMessage;
						pclsRequest->m_strBody.clear();
						pclsRequest->m_iContentLength = 0;
						pclsRequest->m_clsContentType.Clear();
						pclsRequest->m_strSipMethod = "ACK";
						pclsRequest->m_clsCSeq.m_strMethod = "ACK";
						pclsRequest->m_clsCSeq.m_iDigit = pclsMessage->m_clsCSeq.m_iDigit;
						pclsRequest->m_clsTo.m_clsParamList = pclsMessage->m_clsTo.m_clsParamList;
						pclsRequest->m_clsFrom.m_clsParamList = pclsMessage->m_clsFrom.m_clsParamList;

						gclsCallMap.Update( pclsRequest );
						pclsRequest->m_clsRouteList.clear();
						pclsRequest->AddRoute( MCU_IP, MCU_PORT );
						gclsSipStack.SendSipMessage( pclsRequest );
					}
				}
			}
		}
		else if( strstr( pclsMessage->m_clsCallId.m_strName.c_str(), "_copy" ) && pclsMessage->m_iStatusCode == SIP_OK )
		{
			CCallInfo clsCallInfo;

			if( gclsCallMap.Select( pclsMessage, clsCallInfo ) )
			{
				if( clsCallInfo.m_bInviteToMCU == false )
				{
					// 1:1 통화 수신자의 SDP 를 MCU 로 전달한다.
					CSipMessage * pclsRequest = new CSipMessage();
					*pclsRequest = clsCallInfo.m_clsSipMessage;
					pclsRequest->m_strBody = pclsMessage->m_strBody;
					pclsRequest->m_iContentLength = pclsMessage->m_iContentLength;
					pclsRequest->m_clsContentType = pclsMessage->m_clsContentType;
					++pclsRequest->m_clsCSeq.m_iDigit;
					pclsRequest->m_clsFrom = clsCallInfo.m_clsSipMessage.m_clsTo;
					pclsRequest->m_clsFrom.InsertTag();

					gclsCallMap.Update( pclsRequest );
					pclsRequest->m_clsRouteList.clear();
					pclsRequest->AddRoute( MCU_IP, MCU_PORT );
					gclsSipStack.SendSipMessage( pclsRequest );
				}

				// ACK 전송
				{
					CSipMessage * pclsRequest = new CSipMessage();
					*pclsRequest = clsCallInfo.m_clsSipMessage;
					pclsRequest->m_strBody.clear();
					pclsRequest->m_iContentLength = 0;
					pclsRequest->m_clsContentType.Clear();
					pclsRequest->m_strSipMethod = "ACK";
					pclsRequest->m_clsCSeq.m_strMethod = "ACK";
					pclsRequest->m_clsCSeq.m_iDigit = pclsMessage->m_clsCSeq.m_iDigit;
					pclsRequest->m_clsTo.m_clsParamList = pclsMessage->m_clsTo.m_clsParamList;
					pclsRequest->m_clsFrom.m_clsParamList = pclsMessage->m_clsFrom.m_clsParamList;

					gclsSipStack.SendSipMessage( pclsRequest );
				}
			}
		}

		return true;
	}

	return false;
}

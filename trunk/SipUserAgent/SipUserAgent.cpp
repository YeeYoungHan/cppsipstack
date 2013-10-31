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

#include "SipUserAgent.h"
#include "SipRegisterThread.h"
#include "SipUtility.h"
#include "SdpMessage.h"
#include "StringUtility.h"
#include "TimeString.h"
#include "TimeUtility.h"

CSipStack	gclsSipStack;

#include "SipUserAgentCall.hpp"
#include "SipUserAgentSms.hpp"
#include "SipUserAgentPbx.hpp"

#include "SipUserAgentRegister.hpp"
#include "SipUserAgentInvite.hpp"
#include "SipUserAgentBye.hpp"
#include "SipUserAgentCancel.hpp"
#include "SipUserAgentRefer.hpp"
#include "SipUserAgentMessage.hpp"

/**
 * @ingroup SipUserAgent
 * @brief 생성자
 */
CSipUserAgent::CSipUserAgent() : m_pclsCallBack(NULL), m_iSeq(0), m_bStart(false)
{
}

/**
 * @ingroup SipUserAgent
 * @brief 소멸자
 */
CSipUserAgent::~CSipUserAgent()
{
}

/**
 * @ingroup SipUserAgent
 * @brief SIP 로그인 정보를 추가한다.
 * @param clsInfo SIP 로그인 정보 저장 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::InsertRegisterInfo( CSipServerInfo & clsInfo )
{
	if( clsInfo.m_strIp.empty() ) return false;
	if( clsInfo.m_strUserId.empty() ) return false;

	if( clsInfo.m_strDomain.empty() )
	{
		clsInfo.m_strDomain = clsInfo.m_strIp;
	}

	SIP_SERVER_INFO_LIST::iterator	it;
	bool	bFound = false;

	m_clsRegisterMutex.acquire();
	for( it = m_clsRegisterList.begin(); it != m_clsRegisterList.end(); ++it )
	{
		if( it->Equal( clsInfo ) )
		{
			bFound = true;
			break;
		}
	}

	if( bFound == false )
	{
		m_clsRegisterList.push_back( clsInfo );
	}
	m_clsRegisterMutex.release();

	return true;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP 로그인 정보를 수정한다.
 * @param clsInfo SIP 로그인 정보 저장 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::UpdateRegisterInfo( CSipServerInfo & clsInfo )
{
	if( clsInfo.m_strIp.empty() ) return false;
	if( clsInfo.m_strUserId.empty() ) return false;

	SIP_SERVER_INFO_LIST::iterator	it;
	bool	bRes = false;

	m_clsRegisterMutex.acquire();
	for( it = m_clsRegisterList.begin(); it != m_clsRegisterList.end(); ++it )
	{
		if( it->Equal( clsInfo ) )
		{
			bRes = true;
			it->Update( clsInfo );
			break;
		}
	}
	m_clsRegisterMutex.release();

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP 로그인 정보를 삭제한다.
 * @param clsInfo clsInfo SIP 로그인 정보 저장 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::DeleteRegisterInfo( CSipServerInfo & clsInfo )
{
	if( clsInfo.m_strIp.empty() ) return false;
	if( clsInfo.m_strUserId.empty() ) return false;

	SIP_SERVER_INFO_LIST::iterator	it;
	bool	bRes = false;

	m_clsRegisterMutex.acquire();
	for( it = m_clsRegisterList.begin(); it != m_clsRegisterList.end(); ++it )
	{
		if( it->Equal( clsInfo ) )
		{
			bRes = true;

			if( it->m_iLoginTimeout == 0 )
			{
				m_clsRegisterList.erase( it );
			}
			else
			{
				it->m_bDelete = true;
				it->m_iLoginTimeout = 0;
			}

			break;
		}
	}
	m_clsRegisterMutex.release();

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief 모든 SIP 로그인 정보를 삭제한다.
 */
void CSipUserAgent::DeleteRegisterInfoAll( )
{
	SIP_SERVER_INFO_LIST::iterator	it;

	m_clsRegisterMutex.acquire();
	m_clsRegisterList.clear();
	m_clsRegisterMutex.release();
}

/**
 * @ingroup SipUserAgent
 * @brief 로그아웃한다.
 */
void CSipUserAgent::DeRegister( )
{
	SIP_SERVER_INFO_LIST::iterator	it;

	m_clsRegisterMutex.acquire();
	for( it = m_clsRegisterList.begin(); it != m_clsRegisterList.end(); ++it )
	{
		it->m_bDelete = true;
		it->m_iLoginTimeout = 0;
	}
	m_clsRegisterMutex.release();
}

/**
 * @ingroup SipUserAgent
 * @brief SIP stack 을 시작하고 SIP 로그인 쓰레드를 시작한다.
 * @param clsSetup	SIP stack 설정 객체
 * @param pclsCallBack	SIP UserAgent callback 객체
 * @param pclsSecurityCallBack	SIP stack 보안 callback 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::Start( CSipStackSetup & clsSetup, ISipUserAgentCallBack * pclsCallBack, ISipStackSecurityCallBack * pclsSecurityCallBack )
{
	gclsSipStack.AddCallBack( this );

	m_pclsCallBack = pclsCallBack;
	gclsSipStack.SetSecurityCallBack( pclsSecurityCallBack );

	if( gclsSipStack.Start( clsSetup ) == false ) return false;

	StartSipRegisterThread( this );

	m_bStart = true;

	return true;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP stack 을 종료하고 SIP 로그인 쓰레드를 종료한다.
 * @returns true 를 리턴한다.
 */
bool CSipUserAgent::Stop( )
{
	if( m_bStart == false ) return true;

	SIP_SERVER_INFO_LIST::iterator	it;
	int	iCount;

	DeRegister();

	for( int i = 0; i < 10; ++i )
	{
		m_clsRegisterMutex.acquire();
		iCount = (int)m_clsRegisterList.size();
		if( iCount > 0 )
		{
			iCount = 0;

			// 로그인된 개수를 계산한다.
			for( it = m_clsRegisterList.begin(); it != m_clsRegisterList.end(); ++it )
			{
				if( it->m_bLogin ) ++iCount;
			}
		}
		m_clsRegisterMutex.release();

		if( iCount <= 0 ) break;
		sleep(1);
	}

	gclsSipStack.Stop();
	DeleteRegisterInfoAll( );

	m_bStart = false;

	return true;
}


/**
 * @ingroup SipUserAgent
 * @brief SIP 요청 메시지 수신 callback method
 * @param iThreadId		SIP stack 의 UDP 쓰레드 아이디
 * @param pclsMessage 수신된 SIP 요청 메시지
 * @returns SIP 요청 메시지를 처리한 경우 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipUserAgent::RecvRequest( int iThreadId, CSipMessage * pclsMessage )
{
	if( pclsMessage->IsMethod( "INVITE" ) )
	{
		return RecvInviteRequest( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( "BYE" ) )
	{
		return RecvByeRequest( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( "CANCEL" ) )
	{
		return RecvCancelRequest( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( "REFER" ) )
	{
		return RecvReferRequest( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( "MESSAGE" ) )
	{
		return RecvMessageRequest( iThreadId, pclsMessage );
	}

	return false;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP 응답 메시지 수신 callback method
 * @param iThreadId		SIP stack 의 UDP 쓰레드 아이디
 * @param pclsMessage 수신된 SIP 응답 메시지
 * @returns SIP 응답 메시지를 처리한 경우 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipUserAgent::RecvResponse( int iThreadId, CSipMessage * pclsMessage )
{
	if( pclsMessage->IsMethod( "REGISTER" ) )
	{
		return RecvRegisterResponse( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( "INVITE" ) )
	{
		return RecvInviteResponse( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( "BYE" ) )
	{
		return true;
	}
	else if( pclsMessage->IsMethod( "CANCEL" ) )
	{
		return true;
	}
	else if( pclsMessage->IsMethod( "REFER" ) )
	{
		return true;
	}
	else if( pclsMessage->IsMethod( "MESSAGE" ) )
	{
		return true;
	}

	return false;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP 메시지 전송 timeout callback method
 * @param iThreadId		SIP stack 의 UDP 쓰레드 아이디
 * @param pclsMessage 수신된 SIP 응답 메시지
 * @returns SIP 응답 메시지를 처리한 경우 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipUserAgent::SendTimeout( int iThreadId, CSipMessage * pclsMessage )
{
	if( pclsMessage->IsMethod( "ACK" ) )
	{
		std::string	strCallId;

		pclsMessage->GetCallId( strCallId );

		if( m_pclsCallBack ) m_pclsCallBack->EventCallEnd( strCallId.c_str(), SIP_GONE );

		Delete( strCallId.c_str() );
	}

	return false;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP 쓰레드 종료 이벤트 핸들러
 * @param iThreadId 쓰레드 번호
 */
void CSipUserAgent::CallBackThreadEnd( int iThreadId )
{
	if( m_pclsCallBack ) m_pclsCallBack->EventCallBackThreadEnd( iThreadId );
}

/**
 * @ingroup SipUserAgent
 * @brief CSipDialog 에서 SIP INVITE 메시지를 생성하여 전송한다.
 * @param clsDialog 통화 정보 저장 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::SendInvite( CSipDialog & clsDialog )
{
	if( clsDialog.m_strFromId.empty() || clsDialog.m_strToId.empty() ) return false;
	
	SIP_DIALOG_MAP::iterator			itMap;
	char	szTag[SIP_TAG_MAX_SIZE], szBranch[SIP_BRANCH_MAX_SIZE], szCallIdName[SIP_CALL_ID_NAME_MAX_SIZE];
	bool	bInsert = false;
	CSipMessage * pclsMessage = NULL;

	SipMakeTag( szTag, sizeof(szTag) );
	SipMakeBranch( szBranch, sizeof(szBranch) );

	clsDialog.m_strFromTag = szTag;
	clsDialog.m_strViaBranch = szBranch;

	gettimeofday( &clsDialog.m_sttInviteTime, NULL );

	while( 1 )
	{
		SipMakeCallIdName( szCallIdName, sizeof(szCallIdName) );

		clsDialog.m_strCallId = szCallIdName;
		clsDialog.m_strCallId.append( "@" );
		clsDialog.m_strCallId.append( gclsSipStack.m_clsSetup.m_strLocalIp );

		m_clsMutex.acquire();
		itMap = m_clsMap.find( clsDialog.m_strCallId );
		if( itMap == m_clsMap.end() )
		{
			pclsMessage = clsDialog.CreateInvite();
			if( pclsMessage )
			{
				m_clsMap.insert( SIP_DIALOG_MAP::value_type( clsDialog.m_strCallId, clsDialog ) );
				bInsert = true;
			}
		}
		m_clsMutex.release();

		if( bInsert ) break;
	}

	if( gclsSipStack.SendSipMessage( pclsMessage ) == false )
	{
		Delete( clsDialog.m_strCallId.c_str() );
		return false;
	}

	return true;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP Dialog 에 통화 종료 정보를 저장한다.
 * @param pszCallId SIP Call-ID
 * @returns 존재하면 true 를 리턴하고 존재하지 않으면 false 를 리턴한다.
 */
bool CSipUserAgent::SetCallEnd( const char * pszCallId )
{
	SIP_DIALOG_MAP::iterator			itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		gettimeofday( &itMap->second.m_sttEndTime, NULL );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP Dialog 를 삭제한다.
 * @param pszCallId SIP Call-ID
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::Delete( const char * pszCallId )
{
	SIP_DIALOG_MAP::iterator			itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		m_clsMap.erase( itMap );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP INVITE 응답 메시지에 포함된 정보를 CSipDialog 에 저장한다.
 * @param pclsMessage SIP INVITE 응답 메시지
 * @param pclsRtp			remote RTP 정보 저장 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::SetInviteResponse( CSipMessage * pclsMessage, CSipCallRtp * pclsRtp )
{
	std::string	strCallId;

	if( pclsMessage->GetCallId( strCallId ) == false ) return false;

	SIP_DIALOG_MAP::iterator		itMap;
	bool	bFound = false, bReInvite = false;
	CSipMessage *pclsAck = NULL, *pclsInvite = NULL;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strCallId );
	if( itMap != m_clsMap.end() )
	{
		pclsMessage->m_clsTo.SelectParam( "tag", itMap->second.m_strToTag );

		if( pclsRtp )
		{
			itMap->second.SetRemoteRtp( pclsRtp );
		}

		if( pclsMessage->m_iStatusCode >= 200 )
		{
			pclsMessage->m_clsTo.SelectParam( "tag", itMap->second.m_strToTag );
			pclsAck = itMap->second.CreateAck();

			if( pclsMessage->m_iStatusCode >= 200 && pclsMessage->m_iStatusCode < 300 )
			{
				SIP_FROM_LIST::iterator	itContact = pclsMessage->m_clsContactList.begin();
				if( itContact != pclsMessage->m_clsContactList.end() )
				{
					char	szUri[255];

					itContact->m_clsUri.ToString( szUri, sizeof(szUri) );
					itMap->second.m_strContactUri = szUri;
				}

				if( itMap->second.m_sttStartTime.tv_sec == 0 )
				{
					gettimeofday( &itMap->second.m_sttStartTime, NULL );
				}
				else
				{
					bReInvite = true;
				}
			}
			else if( pclsMessage->m_iStatusCode == SIP_UNAUTHORIZED || pclsMessage->m_iStatusCode == SIP_PROXY_AUTHENTICATION_REQUIRED )
			{
				itMap->second.m_strToTag.clear();

				pclsInvite = itMap->second.CreateInvite();
				if( pclsInvite )
				{
					SIP_SERVER_INFO_LIST::iterator itSL;
					const char * pszUserId = pclsMessage->m_clsFrom.m_clsUri.m_strUser.c_str();

					m_clsRegisterMutex.acquire();
					for( itSL = m_clsRegisterList.begin(); itSL != m_clsRegisterList.end(); ++itSL )
					{
						if( !strcmp( itSL->m_strUserId.c_str(), pszUserId ) )
						{
							itSL->AddAuth( pclsInvite, pclsMessage );
							break;
						}
					}
					m_clsRegisterMutex.release();
				}
			}
			else
			{
				gettimeofday( &itMap->second.m_sttEndTime, NULL );
			}
		}

		bFound = true;
	}
	m_clsMutex.release();

	if( pclsAck )
	{
		gclsSipStack.SendSipMessage( pclsAck );
	}

	if( pclsInvite )
	{
		gclsSipStack.SendSipMessage( pclsInvite );
	}

	if( bReInvite ) return false;

	return bFound;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP 메시지에서 RTP 정보를 가져온다.
 * @param pclsMessage SIP 메시지
 * @param clsRtp			RTP 정보 저장 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::GetSipCallRtp( CSipMessage * pclsMessage, CSipCallRtp & clsRtp )
{
	if( pclsMessage->m_clsContentType.IsEqual( "application", "sdp" ) && pclsMessage->m_strBody.empty() == false )
	{
		CSdpMessage clsSdp;

		if( clsSdp.Parse( pclsMessage->m_strBody.c_str(), (int)pclsMessage->m_strBody.length() ) == -1 ) return false;

		clsRtp.m_strIp = clsSdp.m_clsConnection.m_strAddr;

		SDP_MEDIA_LIST::iterator itMedia = clsSdp.m_clsMediaList.begin();
		if( itMedia == clsSdp.m_clsMediaList.end() ) return false;

		clsRtp.m_iPort = itMedia->m_iPort;

		SDP_FMT_LIST::iterator itFmt;
		int iCodec = -1;

		for( itFmt = itMedia->m_clsFmtList.begin(); itFmt != itMedia->m_clsFmtList.end(); ++itFmt )
		{
			iCodec = atoi( itFmt->c_str() );
			if( clsRtp.m_iCodec == -1 )
			{
				clsRtp.m_iCodec = iCodec;
			}

			clsRtp.m_clsCodecList.push_back( iCodec );
		}

		clsRtp.m_eDirection = E_RTP_SEND_RECV;

		SDP_ATTRIBUTE_LIST::iterator	itAttr;

		for( itAttr = itMedia->m_clsAttributeList.begin(); itAttr != itMedia->m_clsAttributeList.end(); ++itAttr )
		{
			if( !strcmp( itAttr->m_strName.c_str(), "sendrecv" ) )
			{
				clsRtp.m_eDirection = E_RTP_SEND_RECV;
				break;
			}
			else if( !strcmp( itAttr->m_strName.c_str(), "sendonly" ) )
			{
				clsRtp.m_eDirection = E_RTP_SEND;
				break;
			}
			else if( !strcmp( itAttr->m_strName.c_str(), "recvonly" ) )
			{
				clsRtp.m_eDirection = E_RTP_RECV;
				break;
			}
			else if( !strcmp( itAttr->m_strName.c_str(), "inactive" ) )
			{
				clsRtp.m_eDirection = E_RTP_INACTIVE;
				break;
			}
		}
		
		return true;
	}

	return false;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP CSeq 헤더에 저장할 번호를 리턴한다.
 * @returns SIP CSeq 헤더에 저장할 번호를 리턴한다.
 */
int CSipUserAgent::GetSeqNum( )
{
	int iSeq;

	m_clsOtherMutex.acquire();
	++m_iSeq;
	if( m_iSeq > 1000000000 )
	{
		m_iSeq = 1;
	}

	iSeq = m_iSeq;
	m_clsOtherMutex.release();

	return iSeq;
}

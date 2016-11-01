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
#include "Log.h"
#include "Random.h"
#include "MemoryDebug.h"

#include "SipUserAgentLogin.hpp"
#include "SipUserAgentSipStack.hpp"
#include "SipUserAgentCall.hpp"
#include "SipUserAgentSms.hpp"
#include "SipUserAgentPbx.hpp"
#include "SipUserAgentMonitor.hpp"

#include "SipUserAgentRegister.hpp"
#include "SipUserAgentInvite.hpp"
#include "SipUserAgentBye.hpp"
#include "SipUserAgentCancel.hpp"
#include "SipUserAgentRefer.hpp"
#include "SipUserAgentNotify.hpp"
#include "SipUserAgentMessage.hpp"
#include "SipUserAgentPrack.hpp"
#include "SipUserAgentOptions.hpp"

/**
 * @ingroup SipUserAgent
 * @brief 생성자
 */
CSipUserAgent::CSipUserAgent() : m_bStopEvent(false), m_pclsCallBack(NULL), m_iSeq(0), m_bStart(false)
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
 * @brief SIP stack 을 시작하고 SIP 로그인 쓰레드를 시작한다.
 * @param clsSetup	SIP stack 설정 객체
 * @param pclsCallBack	SIP UserAgent callback 객체
 * @param pclsSecurityCallBack	SIP stack 보안 callback 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::Start( CSipStackSetup & clsSetup, ISipUserAgentCallBack * pclsCallBack, ISipStackSecurityCallBack * pclsSecurityCallBack )
{
	if( m_bStart ) return false;

	m_clsSipStack.AddCallBack( this );

	m_pclsCallBack = pclsCallBack;
	m_clsSipStack.SetSecurityCallBack( pclsSecurityCallBack );

	if( m_clsSipStack.Start( clsSetup ) == false ) return false;

	StartSipRegisterThread( this );

	m_bStart = true;

	return true;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP stack 을 종료하고 SIP 로그인 쓰레드를 종료한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::Stop( )
{
	if( m_bStart == false ) return false;

	SIP_SERVER_INFO_LIST::iterator	it;
	int	iCount;

	DeRegister();

	for( int i = 0; i < 100; ++i )
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
		MiliSleep(100);
	}

	m_bStopEvent = true;
	m_clsSipStack.Stop();

	// SipRegisterThread 가 종료할 때까지 대기한다.
	for( int i = 0; i < 100; ++i )
	{
		if( m_bStopEvent == false ) break;
		MiliSleep(100);
	}

	DeleteRegisterInfoAll( );

	m_bStart = false;

	return true;
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
		clsDialog.m_strCallId.append( m_clsSipStack.m_clsSetup.m_strLocalIp );

		m_clsDialogMutex.acquire();
		itMap = m_clsDialogMap.find( clsDialog.m_strCallId );
		if( itMap == m_clsDialogMap.end() )
		{
			pclsMessage = clsDialog.CreateInvite();
			if( pclsMessage )
			{
				m_clsDialogMap.insert( SIP_DIALOG_MAP::value_type( clsDialog.m_strCallId, clsDialog ) );
				bInsert = true;
			}
		}
		m_clsDialogMutex.release();

		if( bInsert ) break;
	}

	if( m_clsSipStack.SendSipMessage( pclsMessage ) == false )
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

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		gettimeofday( &itMap->second.m_sttEndTime, NULL );
		bRes = true;
	}
	m_clsDialogMutex.release();

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

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		Delete( itMap );
		bRes = true;
	}
	m_clsDialogMutex.release();

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP Dialog 를 삭제한다.
 * @param itMap dialog map iterator
 */
void CSipUserAgent::Delete( SIP_DIALOG_MAP::iterator & itMap )
{
	if( itMap->second.m_pclsInvite )
	{
		delete itMap->second.m_pclsInvite;
		itMap->second.m_pclsInvite = NULL;
	}

	m_clsDialogMap.erase( itMap );
}

/**
 * @ingroup SipUserAgent
 * @brief SIP INVITE 응답 메시지에 포함된 정보를 CSipDialog 에 저장한다.
 * @param strCallId		SIP Call-ID
 * @param pclsMessage SIP INVITE 응답 메시지
 * @param pclsRtp			remote RTP 정보 저장 객체
 * @param bReINVITE		ReINVITE 유무
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::SetInviteResponse( std::string & strCallId, CSipMessage * pclsMessage, CSipCallRtp * pclsRtp, bool & bReInvite )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool	bFound = false;
	CSipMessage *pclsAck = NULL, *pclsInvite = NULL;

	bReInvite = false;

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( strCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		pclsMessage->m_clsTo.SelectParam( SIP_TAG, itMap->second.m_strToTag );

		if( pclsRtp )
		{
			itMap->second.SetRemoteRtp( pclsRtp );
		}

		if( pclsMessage->m_iStatusCode == SIP_SESSION_PROGRESS || pclsMessage->m_iStatusCode == SIP_RINGING )
		{
			CSipHeader * pclsHeader = pclsMessage->GetHeader( "RSeq" );
			if( pclsHeader && pclsHeader->m_strValue.empty() == false )
			{
				itMap->second.m_iRSeq = atoi( pclsHeader->m_strValue.c_str() );
			}
		}

		if( pclsMessage->m_iStatusCode >= SIP_OK )
		{
			if( pclsMessage->m_iStatusCode != SIP_CONNECT_ERROR )
			{
				pclsAck = itMap->second.CreateAck();
			}

			if( pclsMessage->m_iStatusCode >= SIP_OK && pclsMessage->m_iStatusCode < SIP_MULTIPLE_CHOICES )
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
	m_clsDialogMutex.release();

	if( pclsAck )
	{
		m_clsSipStack.SendSipMessage( pclsAck );
	}

	if( pclsInvite )
	{
		m_clsSipStack.SendSipMessage( pclsInvite );

		// 인증 정보를 포함한 INVITE 메시지를 전송한 경우, 응용으로 callback 호출하지 않는다.
		return false;
	}

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

		if( clsSdp.Parse( pclsMessage->m_strBody.c_str(), (int)pclsMessage->m_strBody.length() ) == -1 ) 
		{
			CLog::Print( LOG_ERROR, "GetSipCallRtp sdp parse error [%s]", pclsMessage->m_strBody.c_str() );
			return false;
		}

		clsRtp.m_strIp = clsSdp.m_clsConnection.m_strAddr;
		SipIpv6Parse( clsRtp.m_strIp );

		SDP_MEDIA_LIST::iterator itMedia = clsSdp.m_clsMediaList.begin();
		if( itMedia == clsSdp.m_clsMediaList.end() ) 
		{
			CLog::Print( LOG_ERROR, "GetSipCallRtp media is not found" );
			return false;
		}

		if( clsRtp.m_strIp.empty() )
		{
			clsRtp.m_strIp = itMedia->m_clsConnection.m_strAddr;
			SipIpv6Parse( clsRtp.m_strIp );
		}

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
			else if( iCodec == 0 )
			{
				// PCMU 가 존재하면 PCMU 를 무조건 선택한다.
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

#ifdef USE_MEDIA_LIST
		clsRtp.m_clsMediaList = clsSdp.m_clsMediaList;
#endif

		return true;
	}

	//CLog::Print( LOG_ERROR, "GetSipCallRtp body is empty" );

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

	m_clsMutex.acquire();
	++m_iSeq;
	if( m_iSeq > 1000000000 )
	{
		m_iSeq = 1;
	}

	iSeq = m_iSeq;
	m_clsMutex.release();

	return iSeq;
}

/**
 * @ingroup SipUserAgent
 * @brief 프로세스가 종료될 때에 최종적으로 실행하여서 openssl 메모리 누수를 출력하지 않는다. 
 */
void CSipUserAgent::Final()
{
	m_clsSipStack.Final();
}

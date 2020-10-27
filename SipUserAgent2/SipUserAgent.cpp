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
#include "SipUserAgentSend.hpp"
#include "SipUserAgentSms.hpp"
#include "SipUserAgentUtil.hpp"
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
 * @brief ������
 */
CSipUserAgent::CSipUserAgent() : m_bStopEvent(false), m_pclsCallBack(NULL), m_iSeq(0), m_bStart(false)
{
}

/**
 * @ingroup SipUserAgent
 * @brief �Ҹ���
 */
CSipUserAgent::~CSipUserAgent()
{
}

/**
 * @ingroup SipUserAgent
 * @brief SIP stack �� �����ϰ� SIP �α��� �����带 �����Ѵ�.
 * @param clsSetup	SIP stack ���� ��ü
 * @param pclsCallBack	SIP UserAgent callback ��ü
 * @param pclsSecurityCallBack	SIP stack ���� callback ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief SIP stack �� �����ϰ� SIP �α��� �����带 �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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

			// �α��ε� ������ ����Ѵ�.
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

	// SipRegisterThread �� ������ ������ ����Ѵ�.
	for( int i = 0; i < 100; ++i )
	{
		if( m_bStopEvent == false ) break;
		MiliSleep(100);
	}

	DeleteRegisterInfoAll( );

	m_clsDialogMutex.acquire();
	m_clsDialogMap.clear();
	m_clsDialogMutex.release();

	m_bStart = false;

	return true;
}

/**
 * @ingroup SipUserAgent
 * @brief CSipDialog ���� SIP INVITE �޽����� �����Ͽ� �����Ѵ�.
 * @param pclsDialog ��ȭ ���� ���� ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::SendInvite( CSipDialog * pclsDialog )
{
	if( pclsDialog->m_strFromId.empty() || pclsDialog->m_strToId.empty() ) return false;
	
	SIP_DIALOG_MAP::iterator			itMap;
	char	szTag[SIP_TAG_MAX_SIZE], szBranch[SIP_BRANCH_MAX_SIZE], szCallIdName[SIP_CALL_ID_NAME_MAX_SIZE];
	bool	bInsert = false;
	CSipMessage * pclsMessage = NULL;

	SipMakeTag( szTag, sizeof(szTag) );
	SipMakeBranch( szBranch, sizeof(szBranch) );

	pclsDialog->m_strFromTag = szTag;
	pclsDialog->m_strViaBranch = szBranch;

	gettimeofday( &pclsDialog->m_sttInviteTime, NULL );

	while( 1 )
	{
		SipMakeCallIdName( szCallIdName, sizeof(szCallIdName) );

		pclsDialog->m_strCallId = szCallIdName;
		pclsDialog->m_strCallId.append( "@" );
		pclsDialog->m_strCallId.append( m_clsSipStack.m_clsSetup.m_strLocalIp );

		m_clsDialogMutex.acquire();
		itMap = m_clsDialogMap.find( pclsDialog->m_strCallId );
		if( itMap == m_clsDialogMap.end() )
		{
			pclsMessage = pclsDialog->CreateInvite();
			if( pclsMessage )
			{
				m_clsDialogMap.insert( SIP_DIALOG_MAP::value_type( pclsDialog->m_strCallId, pclsDialog ) );
				bInsert = true;
			}
		}
		m_clsDialogMutex.release();

		if( bInsert ) break;
	}

	if( m_clsSipStack.SendSipMessage( pclsMessage ) == false )
	{
		Delete( pclsDialog->m_strCallId.c_str() );
		return false;
	}

	return true;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP Dialog �� ��ȭ ���� ������ �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @returns �����ϸ� true �� �����ϰ� �������� ������ false �� �����Ѵ�.
 */
bool CSipUserAgent::SetCallEnd( const char * pszCallId )
{
	SIP_DIALOG_MAP::iterator			itMap;
	bool	bRes = false;

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		gettimeofday( &itMap->second->m_sttEndTime, NULL );
		bRes = true;
	}
	m_clsDialogMutex.release();

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP Dialog �� �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief SIP Dialog �� �����Ѵ�.
 * @param itMap dialog map iterator
 */
void CSipUserAgent::Delete( SIP_DIALOG_MAP::iterator & itMap )
{
	if( itMap->second->m_pclsInvite )
	{
		delete itMap->second->m_pclsInvite;
		itMap->second->m_pclsInvite = NULL;
	}

	delete itMap->second;

	m_clsDialogMap.erase( itMap );
}

/**
 * @ingroup SipUserAgent
 * @brief SIP INVITE ���� �޽����� ���Ե� ������ CSipDialog �� �����Ѵ�.
 * @param strCallId		SIP Call-ID
 * @param pclsMessage SIP INVITE ���� �޽���
 * @param pclsRtp			remote RTP ���� ���� ��ü
 * @param bReINVITE		ReINVITE ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::SetInviteResponse( std::string & strCallId, CSipMessage * pclsMessage, CSipCallRtp * pclsRtp, bool & bReInvite )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool	bFound = false, bStopCall = false;
	CSipMessage *pclsAck = NULL, *pclsInvite = NULL;

	bReInvite = false;

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( strCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		pclsMessage->m_clsTo.SelectParam( SIP_TAG, itMap->second->m_strToTag );

		if( pclsRtp )
		{
			itMap->second->SetRemoteRtp( pclsRtp );
		}

		if( pclsMessage->m_iStatusCode == SIP_SESSION_PROGRESS || pclsMessage->m_iStatusCode == SIP_RINGING )
		{
			CSipHeader * pclsHeader = pclsMessage->GetHeader( "RSeq" );
			if( pclsHeader && pclsHeader->m_strValue.empty() == false )
			{
				itMap->second->m_iRSeq = atoi( pclsHeader->m_strValue.c_str() );
			}
		}

		if( pclsMessage->m_iStatusCode >= SIP_OK )
		{
			if( pclsMessage->m_iStatusCode != SIP_CONNECT_ERROR )
			{
				pclsAck = itMap->second->CreateAck( pclsMessage->m_iStatusCode );
			}

			if( pclsMessage->m_iStatusCode >= SIP_OK && pclsMessage->m_iStatusCode < SIP_MULTIPLE_CHOICES )
			{
				bool bCreateAck = false;

				if( pclsMessage->m_clsRecordRouteList.size() > 0 )
				{
					SIP_FROM_LIST::reverse_iterator itRL;

					itMap->second->m_clsRouteList.clear();

					for( itRL = pclsMessage->m_clsRecordRouteList.rbegin(); itRL != pclsMessage->m_clsRecordRouteList.rend(); ++itRL )
					{
						itMap->second->m_clsRouteList.push_back( *itRL );
					}

					bCreateAck = true;
				}

				SIP_FROM_LIST::iterator	itContact = pclsMessage->m_clsContactList.begin();
				if( itContact != pclsMessage->m_clsContactList.end() )
				{
					char	szUri[255];

					itContact->m_clsUri.ToString( szUri, sizeof(szUri) );
					itMap->second->m_strContactUri = szUri;
					bCreateAck = true;	
				}

				if( bCreateAck )
				{
					if( pclsAck ) delete pclsAck;
					pclsAck = itMap->second->CreateAck( pclsMessage->m_iStatusCode );
				}

				if( itMap->second->m_sttStartTime.tv_sec == 0 )
				{
					gettimeofday( &itMap->second->m_sttStartTime, NULL );
				}
				else
				{
					bReInvite = true;
				}

				if( itMap->second->m_sttCancelTime.tv_sec > 0 )
				{
					bStopCall = true;
				}
			}
			else if( pclsMessage->m_iStatusCode == SIP_UNAUTHORIZED || pclsMessage->m_iStatusCode == SIP_PROXY_AUTHENTICATION_REQUIRED )
			{
				if( itMap->second->m_sttCancelTime.tv_sec == 0 )
				{
					itMap->second->m_strToTag.clear();

					pclsInvite = itMap->second->CreateInvite();
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
			}
			else
			{
				if( itMap->second->m_sttStartTime.tv_sec == 0 )
				{
					gettimeofday( &itMap->second->m_sttEndTime, NULL );

					if( pclsMessage->m_iStatusCode == SIP_MOVED_TEMPORARILY )
					{
						if( itMap->second->m_sttCancelTime.tv_sec == 0 )
						{
							SIP_FROM_LIST::iterator	itContact = pclsMessage->m_clsContactList.begin();
							if( itContact != pclsMessage->m_clsContactList.end() )
							{
								itMap->second->m_strToId = itContact->m_clsUri.m_strUser;
								pclsInvite = itMap->second->CreateInvite();
								if( pclsInvite )
								{
									pclsInvite->m_clsReqUri = itContact->m_clsUri;
								}
							}
						}
					}
				}
				else
				{
					bReInvite = true;
				}
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

		// ���� ������ ������ INVITE �޽����� ������ ���, �������� callback ȣ������ �ʴ´�.
		return false;
	}

	if( bStopCall )
	{
		// CANCEL ���� ��, INVITE 200 OK �����Ͽ����� BYE �� �����Ѵ�.
		StopCall( strCallId.c_str() );
		return false;
	}

	return bFound;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP �޽������� RTP ������ �����´�.
 * @param pclsMessage SIP �޽���
 * @param clsRtp			RTP ���� ���� ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
			if( CSipDialog::IsUseCodec( iCodec ) == false ) continue;

			if( clsRtp.m_iCodec == -1 || iCodec == 0 )
			{
				// PCMU �� �����ϸ� PCMU �� ������ �����Ѵ�.
				clsRtp.m_iCodec = iCodec;
				clsRtp.m_clsCodecList.push_front( iCodec );
			}
			else
			{
				clsRtp.m_clsCodecList.push_back( iCodec );
			}
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
 * @brief SIP CSeq ����� ������ ��ȣ�� �����Ѵ�.
 * @returns SIP CSeq ����� ������ ��ȣ�� �����Ѵ�.
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
 * @brief ���μ����� ����� ���� ���������� �����Ͽ��� openssl �޸� ������ ������� �ʴ´�. 
 */
void CSipUserAgent::Final()
{
	m_clsSipStack.Final();
}
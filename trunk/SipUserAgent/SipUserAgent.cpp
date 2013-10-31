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
 * @brief ������
 */
CSipUserAgent::CSipUserAgent() : m_pclsCallBack(NULL), m_iSeq(0), m_bStart(false)
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
 * @brief SIP �α��� ������ �߰��Ѵ�.
 * @param clsInfo SIP �α��� ���� ���� ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief SIP �α��� ������ �����Ѵ�.
 * @param clsInfo SIP �α��� ���� ���� ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief SIP �α��� ������ �����Ѵ�.
 * @param clsInfo clsInfo SIP �α��� ���� ���� ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief ��� SIP �α��� ������ �����Ѵ�.
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
 * @brief �α׾ƿ��Ѵ�.
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
 * @brief SIP stack �� �����ϰ� SIP �α��� �����带 �����Ѵ�.
 * @param clsSetup	SIP stack ���� ��ü
 * @param pclsCallBack	SIP UserAgent callback ��ü
 * @param pclsSecurityCallBack	SIP stack ���� callback ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief SIP stack �� �����ϰ� SIP �α��� �����带 �����Ѵ�.
 * @returns true �� �����Ѵ�.
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

			// �α��ε� ������ ����Ѵ�.
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
 * @brief SIP ��û �޽��� ���� callback method
 * @param iThreadId		SIP stack �� UDP ������ ���̵�
 * @param pclsMessage ���ŵ� SIP ��û �޽���
 * @returns SIP ��û �޽����� ó���� ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief SIP ���� �޽��� ���� callback method
 * @param iThreadId		SIP stack �� UDP ������ ���̵�
 * @param pclsMessage ���ŵ� SIP ���� �޽���
 * @returns SIP ���� �޽����� ó���� ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief SIP �޽��� ���� timeout callback method
 * @param iThreadId		SIP stack �� UDP ������ ���̵�
 * @param pclsMessage ���ŵ� SIP ���� �޽���
 * @returns SIP ���� �޽����� ó���� ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief SIP ������ ���� �̺�Ʈ �ڵ鷯
 * @param iThreadId ������ ��ȣ
 */
void CSipUserAgent::CallBackThreadEnd( int iThreadId )
{
	if( m_pclsCallBack ) m_pclsCallBack->EventCallBackThreadEnd( iThreadId );
}

/**
 * @ingroup SipUserAgent
 * @brief CSipDialog ���� SIP INVITE �޽����� �����Ͽ� �����Ѵ�.
 * @param clsDialog ��ȭ ���� ���� ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief SIP Dialog �� ��ȭ ���� ������ �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @returns �����ϸ� true �� �����ϰ� �������� ������ false �� �����Ѵ�.
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
 * @brief SIP Dialog �� �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief SIP INVITE ���� �޽����� ���Ե� ������ CSipDialog �� �����Ѵ�.
 * @param pclsMessage SIP INVITE ���� �޽���
 * @param pclsRtp			remote RTP ���� ���� ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief SIP CSeq ����� ������ ��ȣ�� �����Ѵ�.
 * @returns SIP CSeq ����� ������ ��ȣ�� �����Ѵ�.
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

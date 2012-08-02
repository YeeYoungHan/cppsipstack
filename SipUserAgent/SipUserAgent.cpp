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
#include <time.h>

CSipStack	gclsSipStack;

#include "SipUserAgentRegister.hpp"
#include "SipUserAgentInvite.hpp"
#include "SipUserAgentBye.hpp"
#include "SipUserAgentCancel.hpp"

CSipUserAgent::CSipUserAgent() : m_pclsCallBack(NULL)
{
}

CSipUserAgent::~CSipUserAgent()
{
}

bool CSipUserAgent::AddRegisterInfo( CSipServerInfo & clsInfo )
{
	if( clsInfo.m_strIp.empty() ) return false;
	if( clsInfo.m_strUserId.empty() ) return false;

	if( clsInfo.m_strDomain.empty() )
	{
		clsInfo.m_strDomain = clsInfo.m_strIp;
	}

	m_clsRegisterList.push_back( clsInfo );

	return true;
}

bool CSipUserAgent::Start( CSipStackSetup & clsSetup )
{
	if( m_clsRegisterList.size() == 0 ) return false;

	gclsSipStack.AddCallBack( this );

	if( gclsSipStack.Start( clsSetup ) == false ) return false;

	StartSipRegisterThread( this );

	return true;
}

bool CSipUserAgent::Stop( )
{
	gclsSipStack.Stop();

	return false;
}

bool CSipUserAgent::StartCall( const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp, CSipCallRoute * pclsRoute, std::string & strCallId )
{
	if( pszFrom == NULL || pszTo == NULL ) return false;
	if( pclsRtp == NULL || pclsRoute == NULL ) return false;

	CSipDialog	clsDialog;

	clsDialog.m_strFromId = pszFrom;
	clsDialog.m_strToId = pszTo;

	clsDialog.m_strLocalRtpIp = pclsRtp->m_strIp;
	clsDialog.m_iLocalRtpPort = pclsRtp->m_iPort;
	clsDialog.m_iCodec = pclsRtp->m_iCodec;

	clsDialog.m_strContactIp = pclsRoute->m_strDestIp;
	clsDialog.m_iContactPort = pclsRoute->m_iDestPort;

	if( SendInvite( clsDialog ) == false ) return false;
	strCallId = clsDialog.m_strCallId;

	return true;
}

bool CSipUserAgent::StopCall( const char * pszCallId )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool	bRes = false;
	CSipMessage * pclsMessage = NULL;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		if( itMap->second.m_sttStartTime.tv_sec != 0 )
		{
			if( itMap->second.m_sttEndTime.tv_sec == 0 )
			{
				pclsMessage = itMap->second.CreateBye();
				gettimeofday( &itMap->second.m_sttEndTime, NULL );
			}
		}
		else
		{
			if( itMap->second.m_pclsInvite )
			{
				pclsMessage = itMap->second.m_pclsInvite->CreateResponse( SIP_DECLINE );
				gettimeofday( &itMap->second.m_sttEndTime, NULL );
				m_clsMap.erase( itMap );
			}
			else if( itMap->second.m_sttCancelTime.tv_sec == 0 )
			{
				pclsMessage = itMap->second.CreateCancel();
				gettimeofday( &itMap->second.m_sttCancelTime, NULL );
			}
		}
		bRes = true;
	}
	m_clsMutex.release();

	if( pclsMessage )
	{
		gclsSipStack.SendSipMessage( pclsMessage );
	}

	return bRes;
}

bool CSipUserAgent::AcceptCall( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool	bRes = false;
	CSipMessage * pclsMessage = NULL;

	if( pclsRtp == NULL ) return false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		if( itMap->second.m_sttStartTime.tv_sec == 0 )
		{
			itMap->second.m_strLocalRtpIp = pclsRtp->m_strIp;
			itMap->second.m_iLocalRtpPort = pclsRtp->m_iPort;
			itMap->second.m_iCodec = pclsRtp->m_iCodec;

			pclsMessage = itMap->second.m_pclsInvite->CreateResponse( SIP_OK );
			gettimeofday( &itMap->second.m_sttStartTime, NULL );

			itMap->second.AddSdp( pclsMessage );

			delete itMap->second.m_pclsInvite;
			itMap->second.m_pclsInvite = NULL;
		}
	}
	m_clsMutex.release();

	if( pclsMessage )
	{
		gclsSipStack.SendSipMessage( pclsMessage );
	}

	return true;
}

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

	return false;
}

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

	return false;
}

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
		delete pclsMessage;
		Delete( clsDialog.m_strCallId.c_str() );
		return false;
	}

	return true;
}

bool CSipUserAgent::Delete( const char * pszCallId )
{
	SIP_DIALOG_MAP::iterator			itMap;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		m_clsMap.erase( itMap );
	}
	m_clsMutex.release();

	return true;
}

bool CSipUserAgent::SetInviteResponse( CSipMessage * pclsMessage, CSipCallRtp * pclsRtp )
{
	std::string	strCallId;

	if( pclsMessage->GetCallId( strCallId ) == false ) return false;

	SIP_DIALOG_MAP::iterator		itMap;
	bool	bFound = false;
	CSipMessage *pclsAck = NULL, *pclsInvite = NULL;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strCallId );
	if( itMap != m_clsMap.end() )
	{
		pclsMessage->m_clsTo.GetParam( "tag", itMap->second.m_strToTag );

		if( pclsRtp )
		{
			itMap->second.m_strRemoteRtpIp = pclsRtp->m_strIp;
			itMap->second.m_iRemoteRtpPort = pclsRtp->m_iPort;
			itMap->second.m_iCodec = pclsRtp->m_iCodec;
		}

		if( pclsMessage->m_iStatusCode >= 200 )
		{
			pclsMessage->m_clsTo.GetParam( "tag", itMap->second.m_strToTag );
			pclsAck = itMap->second.CreateAck();

			if( pclsMessage->m_iStatusCode >= 200 && pclsMessage->m_iStatusCode < 300 )
			{
				if( itMap->second.m_sttStartTime.tv_sec == 0 )
				{
					gettimeofday( &itMap->second.m_sttStartTime, NULL );
				}

				SIP_FROM_LIST::iterator	itContact = pclsMessage->m_clsContactList.begin();
				if( itContact != pclsMessage->m_clsContactList.end() )
				{
					char	szUri[255];

					itContact->m_clsUri.ToString( szUri, sizeof(szUri) );
					itMap->second.m_strContactUri = szUri;
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

					for( itSL = m_clsRegisterList.begin(); itSL != m_clsRegisterList.end(); ++itSL )
					{
						if( !strcmp( itSL->m_strUserId.c_str(), pszUserId ) )
						{
							itSL->AddAuth( pclsInvite, pclsMessage );
							break;
						}
					}
				}
			}
			else
			{
				gettimeofday( &itMap->second.m_sttEndTime, NULL );
				m_clsMap.erase( itMap );
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

	return bFound;
}

bool CSipUserAgent::GetSipCallRtp( CSipMessage * pclsMessage, CSipCallRtp & clsRtp )
{
	if( pclsMessage->m_clsContentType.IsEqual( "application", "sdp" ) && pclsMessage->m_strBody.empty() == false )
	{
		CSdpMessage clsSdp;

		if( clsSdp.Parse( pclsMessage->m_strBody.c_str(), pclsMessage->m_strBody.length() ) == -1 ) return false;

		clsRtp.m_strIp = clsSdp.m_clsConnection.m_strAddr;

		SDP_MEDIA_LIST::iterator itMedia = clsSdp.m_clsMediaList.begin();
		if( itMedia == clsSdp.m_clsMediaList.end() ) return false;

		clsRtp.m_iPort = itMedia->m_iPort;

		SDP_FMT_LIST::iterator itFmt = itMedia->m_clsFmtList.begin();
		if( itFmt == itMedia->m_clsFmtList.end() ) return false;

		clsRtp.m_iCodec = atoi( itFmt->c_str() );
		
		return true;
	}

	return false;
}

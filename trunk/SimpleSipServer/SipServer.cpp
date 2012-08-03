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

CSipStack	gclsSipStack;
CSipServer gclsSipServer;

CSipServer::CSipServer()
{
}

CSipServer::~CSipServer()
{
}

bool CSipServer::Start( CSipStackSetup & clsSetup )
{
	gclsSipStack.AddCallBack( this );

	if( gclsSipStack.Start( clsSetup ) == false ) return false;

	return true;
}

bool CSipServer::RecvRequest( int iThreadId, CSipMessage * pclsMessage )
{
	if( pclsMessage->IsMethod( "REGISTER" ) )
	{
		// 모든 클라이언트의 로그인을 허용합니다.
		gclsUserMap.Insert( pclsMessage );

		char szToTag[SIP_TAG_MAX_SIZE];

		SipMakeTag( szToTag, sizeof(szToTag) );
		CSipMessage * pclsResponse = pclsMessage->CreateResponse( SIP_OK, szToTag );
		if( pclsResponse )
		{
			return gclsSipStack.SendSipMessage( pclsResponse );
		}
	}
	else
	{
		std::string strToId = pclsMessage->m_clsTo.m_clsUri.m_strUser;
		CUserInfo clsUserInfo;

		if( gclsUserMap.Select( strToId.c_str(), clsUserInfo ) == false )
		{
			char szToTag[SIP_TAG_MAX_SIZE];

			SipMakeTag( szToTag, sizeof(szToTag) );
			CSipMessage * pclsResponse = pclsMessage->CreateResponse( SIP_NOT_FOUND, szToTag );
			if( pclsResponse )
			{
				return gclsSipStack.SendSipMessage( pclsResponse );
			}
		}
		else
		{
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

					if( gclsSipStack.SendSipMessage( pclsRequest ) == false )
					{
						delete pclsRequest;
						return false;
					}

					return true;
				}
			}
		}
	}

	return false;
}

bool CSipServer::RecvResponse( int iThreadId, CSipMessage * pclsMessage )
{
	CSipMessage * pclsResponse = new CSipMessage();
	if( pclsResponse )
	{
		*pclsResponse = *pclsMessage;
		pclsResponse->m_clsViaList.pop_front();

		SIP_FROM_LIST::iterator itContact = pclsResponse->m_clsContactList.begin();
		if( itContact != pclsResponse->m_clsContactList.end() )
		{
			itContact->m_clsUri.m_strHost = gclsSipStack.m_clsSetup.m_strLocalIp;
			itContact->m_clsUri.m_iPort = gclsSipStack.m_clsSetup.m_iLocalUdpPort;
		}

		if( gclsSipStack.SendSipMessage( pclsResponse ) == false )
		{
			delete pclsResponse;
			return false;
		}

		return true;
	}

	return false;
}

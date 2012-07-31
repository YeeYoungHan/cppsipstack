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

#include "SipParserDefine.h"
#include "SipDialogMap.h"
#include "SipUtility.h"
#include "SipUserAgent.h"

CSipDialogMap gclsSipDialogMap;

CSipDialogMap::CSipDialogMap()
{
}

CSipDialogMap::~CSipDialogMap()
{
}

bool CSipDialogMap::SendInvite( CSipDialog & clsDialog )
{
	if( clsDialog.m_strFromId.empty() || clsDialog.m_strToId.empty() ) return false;
	
	SIP_DIALOG_MAP::iterator			itMap;
	char	szTag[SIP_TAG_MAX_SIZE], szBranch[SIP_BRANCH_MAX_SIZE], szCallIdName[SIP_CALL_ID_NAME_MAX_SIZE];
	bool	bInsert = false;

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
			m_clsMap.insert( SIP_DIALOG_MAP::value_type( clsDialog.m_strCallId, clsDialog ) );
			bInsert = true;
		}
		m_clsMutex.release();

		if( bInsert ) break;
	}

	CSipMessage * pclsMessage = clsDialog.CreateInvite();
	if( pclsMessage == NULL )
	{
		Delete( clsDialog.m_strCallId.c_str() );
		return false;
	}

	if( gclsSipStack.SendSipMessage( pclsMessage ) == false )
	{
		delete pclsMessage;
		Delete( clsDialog.m_strCallId.c_str() );
		return false;
	}

	return true;
}

bool CSipDialogMap::SendEnd( const char * pszCallId )
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
			if( itMap->second.m_sttCancelTime.tv_sec == 0 )
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

bool CSipDialogMap::Delete( const char * pszCallId )
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

bool CSipDialogMap::SetInviteResponse( CSipMessage * pclsMessage )
{
	std::string	strCallId;

	if( pclsMessage->GetCallId( strCallId ) == false ) return false;

	SIP_DIALOG_MAP::iterator		itMap;
	bool	bFound = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strCallId );
	if( itMap != m_clsMap.end() )
	{
		if( pclsMessage->m_iStatusCode > 100 && pclsMessage->m_iStatusCode < 200 )
		{
			pclsMessage->m_clsTo.GetParam( "tag", itMap->second.m_strToTag );
		}
		else
		{
			if( pclsMessage->m_iStatusCode >= 200 && pclsMessage->m_iStatusCode < 300 )
			{
				gettimeofday( &itMap->second.m_sttStartTime, NULL );
			}
			else
			{
				gettimeofday( &itMap->second.m_sttEndTime, NULL );
			}

			CSipMessage * pclsAck = itMap->second.CreateAck();
			if( pclsAck )
			{
				gclsSipStack.SendSipMessage( pclsAck );
			}
		}

		bFound = true;
	}
	m_clsMutex.release();

	return bFound;
}

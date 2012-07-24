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

CSipStack	gclsSipStack;

CSipUserAgent::CSipUserAgent(void)
{
}

CSipUserAgent::~CSipUserAgent(void)
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

bool CSipUserAgent::RecvRequest( int iThreadId, CSipMessage * pclsMessage )
{
	

	return false;
}

bool CSipUserAgent::RecvResponse( int iThreadId, CSipMessage * pclsMessage )
{
	if( pclsMessage->IsMethod( "REGISTER" ) )
	{

	}

	return false;
}

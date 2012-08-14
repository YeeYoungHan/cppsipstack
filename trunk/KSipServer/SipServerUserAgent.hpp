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

void CSipServer::EventRegister( CSipServerInfo clsInfo, int iStatus )
{

}

void CSipServer::EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp )
{
	if( gclsUserMap.Select( pszFrom ) == false )
	{
		CSipMessage * pclsInvite = gclsUserAgent.DeleteIncomingCall( pszCallId );
		if( pclsInvite )
		{
			SendUnAuthorizedResponse( pclsInvite );
		}

		return;
	}

	CUserInfo	clsUserInfo;

	if( gclsUserMap.Select( pszTo, clsUserInfo ) == false )
	{
		gclsUserAgent.StopCall( pszCallId );
		return;
	}

	std::string	strCallId;
	CSipCallRoute	clsRoute;

	clsRoute.m_strDestIp = clsUserInfo.m_strIp;
	clsRoute.m_iDestPort = clsUserInfo.m_iPort;

	if( gclsUserAgent.StartCall( pszFrom, pszTo, pclsRtp, &clsRoute, strCallId ) == false )
	{
		gclsUserAgent.StopCall( pszCallId );
		return;
	}

	gclsCallMap.Insert( pszCallId, strCallId.c_str() );
}

void CSipServer::EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp )
{
	std::string	strCallId;

	if( gclsCallMap.Select( pszCallId, strCallId ) == false ) return;

	gclsUserAgent.RingCall( strCallId.c_str(), iSipStatus, pclsRtp );
}

void CSipServer::EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	std::string	strCallId;

	if( gclsCallMap.Select( pszCallId, strCallId ) == false ) return;

	gclsUserAgent.AcceptCall( strCallId.c_str(), pclsRtp );
}

void CSipServer::EventCallEnd( const char * pszCallId, int iSipStatus )
{
	std::string	strCallId;

	if( gclsCallMap.Select( pszCallId, strCallId ) == false ) return;

	gclsUserAgent.StopCall( strCallId.c_str() );
	gclsCallMap.Delete( pszCallId );
}

void CSipServer::EventReInvite( const char * pszCallId, CSipCallRtp * pclsRtp )
{

}

void CSipServer::EventTimer( )
{
	gclsNonceMap.DeleteTimeout( 10 );
}

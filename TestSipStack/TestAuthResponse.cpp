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

#include "SipStack.h"
#include "ServerService.h"

CSipStack gclsSipStack;

class CTestCallBack : public ISipStackCallBack
{
	virtual bool RecvRequest( int iThreadId, CSipMessage * pclsMessage )
	{
		if( pclsMessage->m_clsProxyAuthorizationList.empty() )
		{
			CSipMessage * pclsResponse = pclsMessage->CreateResponseWithToTag( SIP_PROXY_AUTHENTICATION_REQUIRED );
			if( pclsResponse )
			{
				CSipChallenge clsChallenge;

				clsChallenge.m_strRealm = "test.com";
				clsChallenge.m_strQop = "auth";
				clsChallenge.m_strOpaque = "1234";
				clsChallenge.m_strNonce = "4567";
				clsChallenge.m_strAlgorithm = "MD5";

				pclsResponse->m_clsProxyAuthenticateList.push_back( clsChallenge );
				gclsSipStack.SendSipMessage( pclsResponse );
			}
		}
		else
		{
			CSipMessage * pclsResponse = pclsMessage->CreateResponseWithToTag( SIP_OK );
			if( pclsResponse )
			{
				gclsSipStack.SendSipMessage( pclsResponse );
			}
		}

		return true;
	}

	virtual bool RecvResponse( int iThreadId, CSipMessage * pclsMessage )
	{
		return true;
	}

	virtual bool SendTimeout( int iThreadId, CSipMessage * pclsMessage )
	{
		return true;
	}
};

void TestAuthResponse()
{
	CSipStackSetup clsSetup;
	CTestCallBack clsCallBack;

	clsSetup.m_strLocalIp = "127.0.0.1";
	clsSetup.m_iLocalUdpPort = 5080;

	ServerSignal();

	gclsSipStack.AddCallBack( &clsCallBack );

	if( gclsSipStack.Start( clsSetup ) == false )
	{
		printf( "SipServer start error\n" );
		return;
	}

	while( gbStop == false )
	{
		sleep(1);
	}

	gclsSipStack.Stop();
}

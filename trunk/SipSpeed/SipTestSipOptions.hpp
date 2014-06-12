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

static CSipMessage * CreateSipMessage( )
{
	CSipMessage * pclsMessage = new CSipMessage();
	if( pclsMessage == NULL ) return NULL;

	char	szTag[SIP_TAG_MAX_SIZE], szCallIdName[SIP_CALL_ID_NAME_MAX_SIZE];
	std::string strCallId;

	SipMakeTag( szTag, sizeof(szTag) );
	
	while( 1 )
	{
		SipMakeCallIdName( szCallIdName, sizeof(szCallIdName) );

		strCallId = szCallIdName;
		strCallId.append( "@" );
		strCallId.append( gclsSipUserAgent.m_clsSipStack.m_clsSetup.m_strLocalIp );

		if( gclsCallIdMap.Insert( strCallId.c_str() ) )
		{
			break;
		}
	}

	pclsMessage->m_clsCallId.Parse( strCallId.c_str(), (int)strCallId.length() );

	pclsMessage->m_eTransport = E_SIP_UDP;
	pclsMessage->m_strSipMethod = SIP_METHOD_OPTIONS;

	pclsMessage->m_clsReqUri.Set( SIP_PROTOCOL, gclsSetup.m_strCalleeId.c_str(), gclsSetup.m_strSipDomain.c_str(), gclsSetup.m_iSipServerPort );

	pclsMessage->m_clsCSeq.Set( 1, SIP_METHOD_OPTIONS );

	pclsMessage->m_clsFrom.m_clsUri.Set( SIP_PROTOCOL, gclsSetup.m_strCallerId.c_str(), gclsSetup.m_strSipDomain.c_str(), gclsSetup.m_iSipServerPort );
	pclsMessage->m_clsFrom.InsertParam( SIP_TAG, szTag );

	pclsMessage->m_clsTo.m_clsUri.Set( SIP_PROTOCOL, gclsSetup.m_strCalleeId.c_str(), gclsSetup.m_strSipDomain.c_str(), gclsSetup.m_iSipServerPort );

	// Route
	pclsMessage->AddRoute( gclsSetup.m_strSipServerIp.c_str(), gclsSetup.m_iSipServerPort, E_SIP_UDP );

	return pclsMessage;

}

void SipTestSipOptions()
{
	// SIP OPTIONS ���� �׽�Ʈ
	for( int i = 0; i < gclsSetup.m_iCallTotalCount; ++i )
	{
		// ���� ��ȭ ������ ������ ������ ���ų� ū ��쿡�� ����Ѵ�.
		while( gclsCallIdMap.GetCount() >= gclsSetup.m_iCallConcurrentCount )
		{
			Sleep(20);
			if( gbStopTestThread ) break;
		}

		if( gbStopTestThread ) break;

		CSipMessage * pclsMessage = CreateSipMessage( );
		if( pclsMessage == NULL ) break;

		gclsSipUserAgent.m_clsSipStack.SendSipMessage( pclsMessage );
	}

	// ��� ������ ���ŵ� ������ ����Ѵ�.
	while( gclsCallIdMap.GetCount() > 0 )
	{
		Sleep(20);

		if( gbStopTestThread )
		{
			break;
		}
	}

	gclsCallIdMap.DeleteAll();
}

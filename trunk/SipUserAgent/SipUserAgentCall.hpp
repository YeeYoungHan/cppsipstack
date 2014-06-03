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

/**
 * @ingroup SipUserAgent
 * @brief SIP ��ȭ ��û �޽����� �����Ѵ�.
 * @param pszFrom		�߽��� ���̵�
 * @param pszTo			������ ���̵�
 * @param pclsRtp		local RTP ���� ���� ��ü
 * @param pclsRoute SIP �޽��� ������ �ּ� ���� ��ü
 * @param strCallId ������ INVITE �޽����� Call-ID �� ����� ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::StartCall( const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp, CSipCallRoute * pclsRoute, std::string & strCallId )
{
	if( pszFrom == NULL || pszTo == NULL ) return false;
	if( pclsRtp == NULL || pclsRoute == NULL ) return false;

	if( strlen( pszFrom ) == 0 || strlen( pszTo ) == 0 ) return false;

	if( pclsRoute->m_strDestIp.empty() ) return false;
	if( pclsRoute->m_iDestPort <= 0 || pclsRoute->m_iDestPort > 65535 ) return false;

#ifndef USE_TLS
	if( pclsRoute->m_eTransport == E_SIP_TLS ) return false;
#endif

	CSipDialog	clsDialog( &m_clsSipStack );

	clsDialog.m_strFromId = pszFrom;
	clsDialog.m_strToId = pszTo;

	clsDialog.SetLocalRtp( pclsRtp );

	clsDialog.m_strContactIp = pclsRoute->m_strDestIp;
	clsDialog.m_iContactPort = pclsRoute->m_iDestPort;
	clsDialog.m_eTransport = pclsRoute->m_eTransport;

	if( SendInvite( clsDialog ) == false ) return false;
	strCallId = clsDialog.m_strCallId;

	return true;
}

/**
 * @ingroup SipUserAgent
 * @brief ��ȭ�� �����Ѵ�. 
 *				��ȭ ��û�� ������ ������� ������ ��ȭ ��� �޽����� �����Ѵ�.
 *				��ȭ ����Ǿ����� ��ȭ ���� �޽����� �����Ѵ�.
 *				��ȭ ������ ��� ��ȭ ���� ���� �޽����� �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @param iSipCode	��ȭ ��û�� ������ ���� ������ SIP status code
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::StopCall( const char * pszCallId, int iSipCode )
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
				//gettimeofday( &itMap->second.m_sttEndTime, NULL );
				Delete( itMap );
			}
		}
		else
		{
			if( itMap->second.m_pclsInvite )
			{
				if( iSipCode )
				{
					pclsMessage = itMap->second.m_pclsInvite->CreateResponse( iSipCode );
				}
				else
				{
					pclsMessage = itMap->second.m_pclsInvite->CreateResponse( SIP_DECLINE );
				}
				//gettimeofday( &itMap->second.m_sttEndTime, NULL );
				Delete( itMap );
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
		m_clsSipStack.SendSipMessage( pclsMessage );
	}

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief 183 ���� �޽����� �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @param pclsRtp		local RTP ���� ���� ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::RingCall( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool	bRes = false;
	CSipMessage * pclsMessage = NULL;

	if( pclsRtp == NULL ) return false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		if( itMap->second.m_sttStartTime.tv_sec == 0 && itMap->second.m_pclsInvite )
		{
			itMap->second.SetLocalRtp( pclsRtp );

			pclsMessage = itMap->second.m_pclsInvite->CreateResponse( SIP_SESSION_PROGRESS );
			if( pclsMessage )
			{
				if( itMap->second.m_pclsInvite->Is100rel() )
				{
					int		iRSeq = itMap->second.m_pclsInvite->m_clsCSeq.m_iDigit + rand() % 10000;
					char	szRSeq[21];

					snprintf( szRSeq, sizeof(szRSeq), "%d", iRSeq );
					pclsMessage->AddHeader( "RSeq", szRSeq );
				}

				itMap->second.AddSdp( pclsMessage );

				bRes = true;
			}
		}
	}
	m_clsMutex.release();

	if( pclsMessage )
	{
		m_clsSipStack.SendSipMessage( pclsMessage );
	}

	return bRes;

}

/**
 * @ingroup SipUserAgent
 * @brief ���ŵ� ��ȭ�� �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @param pclsRtp		local RTP ���� ���� ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
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
		if( itMap->second.m_sttStartTime.tv_sec == 0 && itMap->second.m_pclsInvite )
		{
			itMap->second.SetLocalRtp( pclsRtp );

			pclsMessage = itMap->second.m_pclsInvite->CreateResponse( SIP_OK );
			if( pclsMessage )
			{
				gettimeofday( &itMap->second.m_sttStartTime, NULL );

				itMap->second.AddSdp( pclsMessage );

				delete itMap->second.m_pclsInvite;
				itMap->second.m_pclsInvite = NULL;

				bRes = true;
			}
		}
	}
	m_clsMutex.release();

	if( pclsMessage )
	{
		m_clsSipStack.SendSipMessage( pclsMessage );
	}

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief	��ȭ ������ �����Ѵ�.
 * @returns ��ȭ ������ �����Ѵ�.
 */
int CSipUserAgent::GetCallCount( )
{
	int iCallCount;

	m_clsMutex.acquire();
	iCallCount = (int)m_clsMap.size();
	m_clsMutex.release();

	return iCallCount;
}

/**
 * @ingroup SipUserAgent
 * @brief ��� ��ȭ�� SIP Call-ID �� ����Ʈ�� �����Ѵ�.
 * @param clsList SIP Call-ID �� ������ ����
 */
void CSipUserAgent::GetCallIdList( SIP_CALL_ID_LIST & clsList )
{
	SIP_DIALOG_MAP::iterator	itMap;

	clsList.clear();

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		clsList.push_back( itMap->first );
	}
	m_clsMutex.release();
}

/**
 * @ingroup SipUserAgent
 * @brief ��� ��ȭ�� �����Ų��.
 */
void CSipUserAgent::StopCallAll( )
{
	SIP_CALL_ID_LIST	clsList;
	SIP_CALL_ID_LIST::iterator	itList;

	GetCallIdList( clsList );

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		StopCall( itList->c_str() );
	}
}

/**
 * @ingroup SipUserAgent
 * @brief SIP INVITE �޽����� �����ϰ� Dialog �� �����Ѵ�. �� �޼ҵ�� SIP INVITE �޽����� ���������� �ʴ´�. 
 *				�� �޼ҵ�� ������ SIP INVITE �޽����� �����Ϸ��� StartCall( const char * pszCallId, CSipMessage * pclsInvite ) �޼ҵ带 �����ؾ� �Ѵ�.
 *				�� �޼ҵ尡 ���ߵ� ������ StartCall( const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp, CSipCallRoute * pclsRoute, std::string & strCallId ) �� ȣ���� ��,
 *				���� ���α׷����� ������ SIP Call-ID �� �ڷᱸ���� �����ϱ� ���� SIP INVITE ���� �޽����� �����ϸ� ���� ���α׷����� �ش� ��ȭ ������
 *				�ڷᱸ���� ��� ���������� ó������ �� �ϱ� ������ �̸� �����ϱ� ���ؼ� SIP INVITE �޽����� SIP Dialog �� �����Ͽ��� SIP Call-ID ��
 *				���� ���α׷��� �����Ͽ� ���� ���α׷� �ڷᱸ���� ������ ��, SIP INVITE �޽����� �����ϱ� �����̴�.
 * @param pszFrom		�߽��� ���̵�
 * @param pszTo			������ ���̵�
 * @param pclsRtp		local RTP ���� ���� ��ü
 * @param pclsRoute SIP �޽��� ������ �ּ� ���� ��ü
 * @param strCallId ������ INVITE �޽����� Call-ID �� ����� ����
 * @param ppclsInvite	������ SIP INVITE �޽����� ����� ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::CreateCall( const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp, CSipCallRoute * pclsRoute, std::string & strCallId, CSipMessage ** ppclsInvite )
{
	if( pszFrom == NULL || pszTo == NULL ) return false;
	if( pclsRtp == NULL || pclsRoute == NULL ) return false;

	if( strlen( pszFrom ) == 0 || strlen( pszTo ) == 0 ) return false;

	if( pclsRoute->m_strDestIp.empty() ) return false;
	if( pclsRoute->m_iDestPort <= 0 || pclsRoute->m_iDestPort > 65535 ) return false;
	if( ppclsInvite == NULL ) return false;

#ifndef USE_TLS
	if( pclsRoute->m_eTransport == E_SIP_TLS ) return false;
#endif

	CSipDialog	clsDialog( &m_clsSipStack );

	clsDialog.m_strFromId = pszFrom;
	clsDialog.m_strToId = pszTo;

	clsDialog.SetLocalRtp( pclsRtp );

	clsDialog.m_strContactIp = pclsRoute->m_strDestIp;
	clsDialog.m_iContactPort = pclsRoute->m_iDestPort;
	clsDialog.m_eTransport = pclsRoute->m_eTransport;

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

	strCallId = clsDialog.m_strCallId;

	*ppclsInvite = pclsMessage;

	return true;
}

/**
 * @ingroup SipUserAgent
 * @brief CreateCall �޼ҵ�� ������ INVITE �޽����� �����Ѵ�.
 * @param pszCallId		SIP Call-ID
 * @param pclsInvite	SIP INVITE �޽���
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::StartCall( const char * pszCallId, CSipMessage * pclsInvite )
{
	if( pszCallId == NULL || pclsInvite == NULL ) return false;

	if( m_clsSipStack.SendSipMessage( pclsInvite ) == false )
	{
		Delete( pszCallId );
		return false;
	}

	return true;
}

/**
 * @ingroup SipUserAgent
 * @brief Blind transfer �� �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @param pszTo			��ȭ ������ ���� ���̵�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::TransferCallBlind( const char * pszCallId, const char * pszTo )
{
	if( pszCallId == NULL || pszTo == NULL ) return false;

	SIP_DIALOG_MAP::iterator		itMap;
	CSipMessage * pclsMessage = NULL;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		pclsMessage = itMap->second.CreateRefer();
	}
	m_clsMutex.release();

	if( pclsMessage == NULL ) return false;

	char szUri[1024];

	snprintf( szUri, sizeof(szUri), "<sip:%s@%s:%d>", pszTo, m_clsSipStack.m_clsSetup.m_strLocalIp.c_str(), m_clsSipStack.m_clsSetup.m_iLocalUdpPort );
	pclsMessage->AddHeader( "Refer-To", szUri );

	return m_clsSipStack.SendSipMessage( pclsMessage );
}

/**
 * @ingroup SipUserAgent
 * @brief Screened / Unscreened transfer �� �����Ѵ�.
 * @param pszCallId		SIP Call-ID
 * @param pszToCallId	��ȭ ������ ���� SIP Call-ID
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::TransferCall( const char * pszCallId, const char * pszToCallId )
{
	if( pszCallId == NULL || pszToCallId == NULL ) return false;

	SIP_DIALOG_MAP::iterator		itMap;
	CSipMessage * pclsMessage = NULL;
	std::string strReplaces, strToId;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszToCallId );
	if( itMap != m_clsMap.end() )
	{
		strToId = itMap->second.m_strToId;

		strReplaces = pszToCallId;
		ReplaceString( strReplaces, "@", "%40" );
		strReplaces.append( "%3B" );
		strReplaces.append( "to-tag%3D" );
		strReplaces.append( itMap->second.m_strToTag );
		strReplaces.append( "%3B" );
		strReplaces.append( "from-tag%3D" );
		strReplaces.append( itMap->second.m_strFromTag );

		itMap = m_clsMap.find( pszCallId );
		if( itMap != m_clsMap.end() )
		{
			pclsMessage = itMap->second.CreateRefer();
		}
	}
	m_clsMutex.release();

	if( pclsMessage == NULL ) return false;

	char szUri[1024];

	snprintf( szUri, sizeof(szUri), "<sip:%s@%s:%d?Replaces=%s>"
		, strToId.c_str(), m_clsSipStack.m_clsSetup.m_strLocalIp.c_str(), m_clsSipStack.m_clsSetup.m_iLocalUdpPort, strReplaces.c_str() );
	pclsMessage->AddHeader( "Refer-To", szUri );

	return m_clsSipStack.SendSipMessage( pclsMessage );
}

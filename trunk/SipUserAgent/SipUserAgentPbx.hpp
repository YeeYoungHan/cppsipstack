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
 * @brief SIP ��ȭ ��û INVITE �޽����� �˻��Ѵ�.
 * @param pszCallId SIP Call-ID
 * @returns �����ϸ� SIP ��ȭ ��û INVITE �޽����� �����ϰ� �����ϸ� NULL �� �����Ѵ�.
 */
CSipMessage * CSipUserAgent::DeleteIncomingCall( const char * pszCallId )
{
	SIP_DIALOG_MAP::iterator		itMap;
	CSipMessage * pclsMessage = NULL;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		if( itMap->second.m_sttStartTime.tv_sec == 0 )
		{
			if( itMap->second.m_pclsInvite )
			{
				pclsMessage = itMap->second.m_pclsInvite;
				itMap->second.m_pclsInvite = NULL;
				Delete( itMap );
			}
		}
	}
	m_clsMutex.release();

	return pclsMessage;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP ��ȭ ��û�� ���� Ring / Session Progress ���� �޽����� �����Ѵ�.
 * @param pszCallId		SIP Call-ID
 * @param iSipStatus	SIP ���� �ڵ�
 * @param pclsRtp			local RTP ���� ���� ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::RingCall( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp )
{
	SIP_DIALOG_MAP::iterator		itMap;
	CSipMessage * pclsMessage = NULL;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		if( itMap->second.m_sttStartTime.tv_sec == 0 )
		{
			if( itMap->second.m_pclsInvite )
			{
				pclsMessage = itMap->second.m_pclsInvite->CreateResponse( iSipStatus );

				if( pclsRtp )
				{
					itMap->second.SetLocalRtp( pclsRtp );
					itMap->second.AddSdp( pclsMessage );
				}

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
 * @brief SIP Call-ID �� ��ȭ�� �˻��� ��, �˻��� ����� peer RTP ������ �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @param pclsRtp		peer RTP ������ ������ ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::GetRemoteCallRtp( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		bRes = itMap->second.SelectRemoteRtp( pclsRtp );
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP Call-ID �� ��ȭ�� �˻��� ��, �˻��� ����� peer ���̵� �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @param strToId		peer ���̵� ������ ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::GetToId( const char * pszCallId, std::string & strToId )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool	bRes = false;

	strToId.clear();

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		strToId = itMap->second.m_strToId;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP Call-ID �� ��ȭ�� �˻��� ��, �˻��� ����� my ���̵� �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @param strFromId		my ���̵� ������ ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::GetFromId( const char * pszCallId, std::string & strFromId )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool	bRes = false;

	strFromId.clear();

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		strFromId = itMap->second.m_strFromId;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP Call-ID �� ��ȭ�� �˻��� ��, �˻��� ����� CDR ������ �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @param pclsCdr		CDR ���� ���� ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::GetCdr( const char * pszCallId, CSipCdr * pclsCdr )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		itMap->second.GetCdr( pclsCdr );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @brief ��ȭ�� ���� ��û ������ Ȯ���Ѵ�.
 * @param pszCallId SIP Call-ID
 * @param pszTo			SIP TO ���̵�
 * @returns ��ȭ�� ����Ǿ����� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipUserAgent::IsRingCall( const char * pszCallId, const char * pszTo )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		if( itMap->second.IsConnected() == false )
		{
			if( pszTo )
			{
				if( !strcmp( pszTo, itMap->second.m_strToId.c_str() ) )
				{
					bRes = true;
				}
			}
			else
			{
				bRes = true;
			}
		}
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @brief Dialog �� RSeq �� �����ϴ��� �˻��Ѵ�.
 * @param pszCallId SIP Call-ID
 * @returns Dialog �� RSeq �� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipUserAgent::HasRSeq( const char * pszCallId )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		if( itMap->second.m_iRSeq != -1 )
		{
			bRes = true;
		}
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief ReINVITE �޽����� �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @param pclsRtp		local RTP ���� ���� ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::SendReInvite( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	SIP_DIALOG_MAP::iterator		itMap;
	CSipMessage * pclsRequest = NULL;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		itMap->second.SetLocalRtp( pclsRtp );
		pclsRequest = itMap->second.CreateInvite();
		bRes = true;
	}
	m_clsMutex.release();

	if( pclsRequest )
	{
		m_clsSipStack.SendSipMessage( pclsRequest );
	}

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief Blind Transfer ���� ���Ǵ� NOTIFY �޽����� �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @param iSipCode	INVITE ���� �޽����� SIP status code
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::SendNotify( const char * pszCallId, int iSipCode )
{
	SIP_DIALOG_MAP::iterator		itMap;
	CSipMessage * pclsRequest = NULL;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		pclsRequest = itMap->second.CreateNotify();
		bRes = true;
	}
	m_clsMutex.release();

	if( pclsRequest )
	{
		char	szBuf[255];

		pclsRequest->m_clsContentType.Set( "message", "sipfrag" );
		pclsRequest->m_clsContentType.InsertParam( "version", "2.0" );
		pclsRequest->AddHeader( "Event", "refer" );

		if( iSipCode >= 200 )
		{
			pclsRequest->AddHeader( "Subscription-State",  "terminated" );
		}
		else
		{
			pclsRequest->AddHeader( "Subscription-State",  "active" );
		}

		pclsRequest->m_iContentLength = snprintf( szBuf, sizeof(szBuf), "SIP/2.0 %d %s", iSipCode, GetReasonPhrase( iSipCode ) );
		pclsRequest->m_strBody = szBuf;

		m_clsSipStack.SendSipMessage( pclsRequest );
	}

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief �ڷᱸ�� ����͸��� ���� ���ڿ��� �����Ѵ�.
 * @param strBuf �ڷᱸ�� ����͸��� ���� ���ڿ� ����
 */
void CSipUserAgent::GetString( std::string & strBuf )
{
	SIP_DIALOG_MAP::iterator		itMap;
	char	szTemp[51];

	strBuf.clear();

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		strBuf.append( itMap->first );
		strBuf.append( MR_COL_SEP );

		strBuf.append( itMap->second.m_strFromId );
		strBuf.append( MR_COL_SEP );

		strBuf.append( itMap->second.m_strToId );
		strBuf.append( MR_COL_SEP );

		strBuf.append( itMap->second.m_strContactIp );
		snprintf( szTemp, sizeof(szTemp), ":%d", itMap->second.m_iContactPort );
		strBuf.append( szTemp );
		strBuf.append( MR_COL_SEP );

		if( itMap->second.m_iLocalRtpPort > 0 )
		{
			strBuf.append( itMap->second.m_strLocalRtpIp );
			snprintf( szTemp, sizeof(szTemp), ":%d", itMap->second.m_iLocalRtpPort );
			strBuf.append( szTemp );
		}
		strBuf.append( MR_COL_SEP );

		if( itMap->second.m_iRemoteRtpPort > 0 )
		{
			strBuf.append( itMap->second.m_strRemoteRtpIp );
			snprintf( szTemp, sizeof(szTemp), ":%d", itMap->second.m_iRemoteRtpPort );
			strBuf.append( szTemp );
		}
		strBuf.append( MR_COL_SEP );

		GetDateTimeString( itMap->second.m_sttInviteTime.tv_sec, szTemp, sizeof(szTemp) );
		strBuf.append( szTemp );
		strBuf.append( MR_COL_SEP );

		if( itMap->second.m_sttStartTime.tv_sec )
		{
			GetDateTimeString( itMap->second.m_sttStartTime.tv_sec, szTemp, sizeof(szTemp) );
			strBuf.append( szTemp );
		}
		strBuf.append( MR_COL_SEP );

		if( itMap->second.m_sttEndTime.tv_sec )
		{
			GetDateTimeString( itMap->second.m_sttEndTime.tv_sec, szTemp, sizeof(szTemp) );
			strBuf.append( szTemp );
		}

		strBuf.append( MR_ROW_SEP );
	}
	m_clsMutex.release();
}

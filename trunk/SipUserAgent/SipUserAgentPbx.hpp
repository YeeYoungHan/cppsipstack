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

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
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
	m_clsDialogMutex.release();

	return pclsMessage;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP ��ȭ ��û�� ���� Ring / Session Progress ���� �޽����� �����Ѵ�. IP-PBX ���� Ring / Session Progress �޽����� ������ ���� ���ȴ�.
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

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		if( itMap->second.m_sttStartTime.tv_sec == 0 && itMap->second.m_pclsInvite )
		{
			pclsMessage = itMap->second.m_pclsInvite->CreateResponse( iSipStatus );

			if( pclsRtp )
			{
				itMap->second.SetLocalRtp( pclsRtp );
				itMap->second.AddSdp( pclsMessage );
			}

			if( itMap->second.m_iRSeq != -1 )
			{
				pclsMessage->AddHeader( "Allow", "PRACK, INVITE, ACK, BYE, CANCEL, REFER, NOTIFY, MESSAGE" );
				pclsMessage->AddHeader( "Require", "100rel" );

				char szRSeq[21];

				snprintf( szRSeq, sizeof(szRSeq), "%d", itMap->second.m_iRSeq );
				pclsMessage->AddHeader( "RSeq", szRSeq );
			}

			bRes = true;
		}
	}
	m_clsDialogMutex.release();

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

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		bRes = itMap->second.SelectRemoteRtp( pclsRtp );
	}
	m_clsDialogMutex.release();

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

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		strToId = itMap->second.m_strToId;
		bRes = true;
	}
	m_clsDialogMutex.release();

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

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		strFromId = itMap->second.m_strFromId;
		bRes = true;
	}
	m_clsDialogMutex.release();

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

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		itMap->second.GetCdr( pclsCdr );
		bRes = true;
	}
	m_clsDialogMutex.release();

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

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
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
	m_clsDialogMutex.release();

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP INVITE �޽����� ������ ���, �ش� SIP INVITE �޽������� ��� �̸��� �˻��Ͽ��� �̿� ���� ���� �����Ѵ�.
 *				��� ����� �˻��ϴ� ���� �ƴϰ� CSipMessage �� m_clsHeaderList �� ����� ����� �˻��Ѵ�.
 * @param pszCallId			SIP Call-ID
 * @param pszHeaderName ��� �̸�
 * @param strValue			����� ���� ������ ����
 * @returns �˻��� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipUserAgent::GetInviteHeaderValue( const char * pszCallId, const char * pszHeaderName, std::string & strValue )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool bRes = false;

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		if( itMap->second.m_pclsInvite )
		{
			CSipHeader * pclsHeader = itMap->second.m_pclsInvite->GetHeader( pszHeaderName );
			if( pclsHeader )
			{
				strValue = pclsHeader->m_strValue;
				bRes = true;
			}
		}
	}
	m_clsDialogMutex.release();

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief Dialog �� RSeq ���� �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @returns Dialog �� RSeq �� �����ϸ� RSeq ���� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipUserAgent::GetRSeq( const char * pszCallId )
{
	SIP_DIALOG_MAP::iterator		itMap;
	int iRSeq = -1;

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		iRSeq = itMap->second.m_iRSeq;
	}
	m_clsDialogMutex.release();

	return iRSeq;
}

/**
 * @ingroup SipUserAgent
 * @brief Dialog �� RSeq ���� �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @param iRSeq RSeq ��
 */
void CSipUserAgent::SetRSeq( const char * pszCallId, int iRSeq )
{
	SIP_DIALOG_MAP::iterator		itMap;

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		itMap->second.m_iRSeq = iRSeq;
	}
	m_clsDialogMutex.release();
}

/**
 * @ingroup SipUserAgent
 * @brief 100rel ����� Ȱ��ȭ ������ �˻��Ѵ�.
 * @param pszCallId SIP Call-ID
 * @returns 100rel ����� Ȱ��ȭ�Ǿ� ������ true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipUserAgent::Is100rel( const char * pszCallId )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool	b100rel = false;

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		b100rel = itMap->second.m_b100rel;
		if( b100rel == false && itMap->second.m_pclsInvite )
		{
			b100rel = itMap->second.m_pclsInvite->Is100rel();
		}
	}
	m_clsDialogMutex.release();

	return b100rel;
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

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		itMap->second.SetLocalRtp( pclsRtp );
		pclsRequest = itMap->second.CreateInvite();
		bRes = true;
	}
	m_clsDialogMutex.release();

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

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		pclsRequest = itMap->second.CreateNotify();
		bRes = true;
	}
	m_clsDialogMutex.release();

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

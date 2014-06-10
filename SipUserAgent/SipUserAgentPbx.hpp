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
 * @brief SIP 통화 요청 INVITE 메시지를 검색한다.
 * @param pszCallId SIP Call-ID
 * @returns 성공하면 SIP 통화 요청 INVITE 메시지를 리턴하고 실패하면 NULL 를 리턴한다.
 */
CSipMessage * CSipUserAgent::DeleteIncomingCall( const char * pszCallId )
{
	SIP_DIALOG_MAP::iterator		itMap;
	CSipMessage * pclsMessage = NULL;

	m_clsMutex.acquire();
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
	m_clsMutex.release();

	return pclsMessage;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP 통화 요청에 대한 Ring / Session Progress 응답 메시지를 전송한다.
 * @param pszCallId		SIP Call-ID
 * @param iSipStatus	SIP 응답 코드
 * @param pclsRtp			local RTP 정보 저장 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::RingCall( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp )
{
	SIP_DIALOG_MAP::iterator		itMap;
	CSipMessage * pclsMessage = NULL;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
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
 * @brief SIP Call-ID 로 통화를 검색한 후, 검색된 결과로 peer RTP 정보를 저장한다.
 * @param pszCallId SIP Call-ID
 * @param pclsRtp		peer RTP 정보를 저장할 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::GetRemoteCallRtp( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		bRes = itMap->second.SelectRemoteRtp( pclsRtp );
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP Call-ID 로 통화를 검색한 후, 검색된 결과로 peer 아이디를 저장한다.
 * @param pszCallId SIP Call-ID
 * @param strToId		peer 아이디를 저장할 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::GetToId( const char * pszCallId, std::string & strToId )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool	bRes = false;

	strToId.clear();

	m_clsMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		strToId = itMap->second.m_strToId;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP Call-ID 로 통화를 검색한 후, 검색된 결과로 my 아이디를 저장한다.
 * @param pszCallId SIP Call-ID
 * @param strFromId		my 아이디를 저장할 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::GetFromId( const char * pszCallId, std::string & strFromId )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool	bRes = false;

	strFromId.clear();

	m_clsMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		strFromId = itMap->second.m_strFromId;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP Call-ID 로 통화를 검색한 후, 검색된 결과의 CDR 정보를 저장한다.
 * @param pszCallId SIP Call-ID
 * @param pclsCdr		CDR 정보 저장 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::GetCdr( const char * pszCallId, CSipCdr * pclsCdr )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		itMap->second.GetCdr( pclsCdr );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @brief 통화가 연결 요청 중인지 확인한다.
 * @param pszCallId SIP Call-ID
 * @param pszTo			SIP TO 아이디
 * @returns 통화가 연결되었으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipUserAgent::IsRingCall( const char * pszCallId, const char * pszTo )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
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
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief Dialog 의 RSeq 값을 리턴한다.
 * @param pszCallId SIP Call-ID
 * @returns Dialog 에 RSeq 가 존재하면 RSeq 값을 리턴하고 그렇지 않으면 -1 을 리턴한다.
 */
int CSipUserAgent::GetRSeq( const char * pszCallId )
{
	SIP_DIALOG_MAP::iterator		itMap;
	int iRSeq = -1;

	m_clsMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		iRSeq = itMap->second.m_iRSeq;
	}
	m_clsMutex.release();

	return iRSeq;
}

/**
 * @ingroup SipUserAgent
 * @brief Dialog 의 RSeq 값을 설정한다.
 * @param pszCallId SIP Call-ID
 * @param iRSeq RSeq 값
 */
void CSipUserAgent::SetRSeq( const char * pszCallId, int iRSeq )
{
	SIP_DIALOG_MAP::iterator		itMap;

	m_clsMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		itMap->second.m_iRSeq = iRSeq;
	}
	m_clsMutex.release();
}

/**
 * @ingroup SipUserAgent
 * @brief 100rel 기능이 활성화 유무를 검색한다.
 * @param pszCallId SIP Call-ID
 * @returns 100rel 기능이 활성화되어 있으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipUserAgent::Is100rel( const char * pszCallId )
{
	SIP_DIALOG_MAP::iterator		itMap;
	bool	b100rel = false;

	m_clsMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		b100rel = itMap->second.m_b100rel;
		if( b100rel == false && itMap->second.m_pclsInvite )
		{
			b100rel = itMap->second.m_pclsInvite->Is100rel();
		}
	}
	m_clsMutex.release();

	return b100rel;
}

/**
 * @ingroup SipUserAgent
 * @brief ReINVITE 메시지를 전송한다.
 * @param pszCallId SIP Call-ID
 * @param pclsRtp		local RTP 정보 저장 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::SendReInvite( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	SIP_DIALOG_MAP::iterator		itMap;
	CSipMessage * pclsRequest = NULL;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
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
 * @brief Blind Transfer 에서 사용되는 NOTIFY 메시지를 전송한다.
 * @param pszCallId SIP Call-ID
 * @param iSipCode	INVITE 응답 메시지의 SIP status code
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::SendNotify( const char * pszCallId, int iSipCode )
{
	SIP_DIALOG_MAP::iterator		itMap;
	CSipMessage * pclsRequest = NULL;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsDialogMap.find( pszCallId );
	if( itMap != m_clsDialogMap.end() )
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
 * @brief 자료구조 모니터링을 위한 문자열을 저장한다.
 * @param strBuf 자료구조 모니터링을 위한 문자열 변수
 */
void CSipUserAgent::GetString( CMonitorString & strBuf )
{
	SIP_DIALOG_MAP::iterator		itMap;

	strBuf.Clear();

	m_clsMutex.acquire();
	for( itMap = m_clsDialogMap.begin(); itMap != m_clsDialogMap.end(); ++itMap )
	{
		strBuf.AddCol( itMap->first );
		strBuf.AddCol( itMap->second.m_strFromId );
		strBuf.AddCol( itMap->second.m_strToId );
		strBuf.AddCol( itMap->second.m_strContactIp, itMap->second.m_iContactPort );

		if( itMap->second.m_iLocalRtpPort > 0 )
		{
			strBuf.AddCol( itMap->second.m_strLocalRtpIp, itMap->second.m_iLocalRtpPort );
		}
		else
		{
			strBuf.AddCol( "" );
		}

		if( itMap->second.m_iRemoteRtpPort > 0 )
		{
			strBuf.AddCol( itMap->second.m_strRemoteRtpIp, itMap->second.m_iRemoteRtpPort );
		}
		else
		{
			strBuf.AddCol( "" );
		}

		strBuf.AddCol( (time_t)itMap->second.m_sttInviteTime.tv_sec );

		if( itMap->second.m_sttStartTime.tv_sec )
		{
			strBuf.AddCol( (time_t)itMap->second.m_sttStartTime.tv_sec );
		}
		else
		{
			strBuf.AddCol( "" );
		}

		if( itMap->second.m_sttEndTime.tv_sec )
		{
			strBuf.AddRow( (time_t)itMap->second.m_sttEndTime.tv_sec );
		}
		else
		{
			strBuf.AddRow( "" );
		}
	}
	m_clsMutex.release();
}

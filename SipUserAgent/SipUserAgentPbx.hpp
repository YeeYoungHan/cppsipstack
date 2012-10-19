
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
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		if( itMap->second.m_sttStartTime.tv_sec == 0 )
		{
			if( itMap->second.m_pclsInvite )
			{
				pclsMessage = itMap->second.m_pclsInvite;
				itMap->second.m_pclsInvite = NULL;
				m_clsMap.erase( itMap );
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
		gclsSipStack.SendSipMessage( pclsMessage );
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
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		pclsRtp->m_strIp = itMap->second.m_strRemoteRtpIp;
		pclsRtp->m_iPort = itMap->second.m_iRemoteRtpPort;
		pclsRtp->m_iCodec = itMap->second.m_iCodec;
		pclsRtp->m_eDirection = itMap->second.m_eRemoteDirection;
		bRes = true;
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
		gclsSipStack.SendSipMessage( pclsRequest );
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

		gclsSipStack.SendSipMessage( pclsRequest );
	}

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief 자료구조 모니터링을 위한 문자열을 저장한다.
 * @param strBuf 자료구조 모니터링을 위한 문자열 변수
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

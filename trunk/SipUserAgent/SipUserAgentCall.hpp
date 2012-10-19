/**
 * @ingroup SipUserAgent
 * @brief SIP 통화 요청 메시지를 전송한다.
 * @param pszFrom		발신자 아이디
 * @param pszTo			수신자 아이디
 * @param pclsRtp		local RTP 정보 저장 객체
 * @param pclsRoute SIP 메시지 목적지 주소 저장 객체
 * @param strCallId 생성된 INVITE 메시지의 Call-ID 가 저장될 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::StartCall( const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp, CSipCallRoute * pclsRoute, std::string & strCallId )
{
	if( pszFrom == NULL || pszTo == NULL ) return false;
	if( pclsRtp == NULL || pclsRoute == NULL ) return false;

	CSipDialog	clsDialog;

	clsDialog.m_strFromId = pszFrom;
	clsDialog.m_strToId = pszTo;

	clsDialog.SetLocalRtp( pclsRtp );

	clsDialog.m_strContactIp = pclsRoute->m_strDestIp;
	clsDialog.m_iContactPort = pclsRoute->m_iDestPort;

	if( SendInvite( clsDialog ) == false ) return false;
	strCallId = clsDialog.m_strCallId;

	return true;
}

/**
 * @ingroup SipUserAgent
 * @brief 통화를 종료한다. 
 *				통화 요청을 보내고 연결되지 않으면 통화 취소 메시지를 전송한다.
 *				통화 연결되었으면 통화 종료 메시지를 전송한다.
 *				통화 수락인 경우 통화 거절 응답 메시지를 전송한다.
 * @param pszCallId SIP Call-ID
 * @param iSipCode	통화 요청을 거절할 때에 전송할 SIP status code
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
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
				gettimeofday( &itMap->second.m_sttEndTime, NULL );
				m_clsMap.erase( itMap );
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
				gettimeofday( &itMap->second.m_sttEndTime, NULL );
				m_clsMap.erase( itMap );
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
		gclsSipStack.SendSipMessage( pclsMessage );
	}

	return bRes;
}

/**
 * @ingroup SipUserAgent
 * @brief 수신된 통화를 수락한다.
 * @param pszCallId SIP Call-ID
 * @param pclsRtp		local RTP 정보 저장 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
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
			gettimeofday( &itMap->second.m_sttStartTime, NULL );

			itMap->second.AddSdp( pclsMessage );

			delete itMap->second.m_pclsInvite;
			itMap->second.m_pclsInvite = NULL;

			bRes = true;
		}
	}
	m_clsMutex.release();

	if( pclsMessage )
	{
		gclsSipStack.SendSipMessage( pclsMessage );
	}

	return bRes;
}

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

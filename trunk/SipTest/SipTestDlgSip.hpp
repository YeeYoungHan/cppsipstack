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

LRESULT CSipTestDlg::OnSipMessage( WPARAM wParam, LPARAM lParam )
{
	return m_clsSipUserAgentMFC.OnSipMessage( wParam, lParam );
}

/**
 * @ingroup SipTest
 * @brief SIP REGISTER ���� �޽��� ���� �̺�Ʈ �ڵ鷯
 * @param pclsInfo	SIP REGISTER ���� �޽����� ������ IP-PBX ���� ���� ��ü
 * @param iStatus		SIP REGISTER ���� �ڵ�
 */
void CSipTestDlg::EventRegister( CSipServerInfo * pclsInfo, int iStatus )
{
	std::string	strCommand;

	if( pclsInfo->m_bLogin )
	{
		strCommand = "login";
	}
	else
	{
		strCommand = "logout";
	}

	if( !strcmp( pclsInfo->m_strUserId.c_str(), m_strCallerId ) )
	{
		if( iStatus == SIP_OK )
		{
			if( pclsInfo->m_bLogin )
			{
				m_bCallerLogin = true;
			}
			else
			{
				m_bCallerLogin = false;
			}

			SetLog( "Caller(%s) %s success", m_strCallerId, strCommand.c_str() );
		}
		else
		{
			m_bCallerLogin = false;
			SetLog( "Caller(%s) %s error(%d)", m_strCallerId, strCommand.c_str(), iStatus );
		}
	}
	else if( !strcmp( pclsInfo->m_strUserId.c_str(), m_strCalleeId ) )
	{
		if( iStatus == SIP_OK )
		{
			if( pclsInfo->m_bLogin )
			{
				m_bCalleeLogin = true;
			}
			else
			{
				m_bCalleeLogin = false;
			}

			SetLog( "Callee(%s) %s success", m_strCalleeId, strCommand.c_str() );
		}
		else
		{
			m_bCalleeLogin = false;
			SetLog( "Callee(%s) %s error(%d)", m_strCalleeId, strCommand.c_str(), iStatus );
		}
	}
	else if( !strcmp( pclsInfo->m_strUserId.c_str(), m_strCalleeId2 ) )
	{
		if( iStatus == SIP_OK )
		{
			if( pclsInfo->m_bLogin )
			{
				m_bCallee2Login = true;
			}
			else
			{
				m_bCallee2Login = false;
			}

			SetLog( "Callee(%s) %s success", m_strCalleeId2, strCommand.c_str() );
		}
		else
		{
			m_bCallee2Login = false;
			SetLog( "Callee(%s) %s error(%d)", m_strCalleeId2, strCommand.c_str(), iStatus );
		}
	}

	// Caller, Callee �� ��� �α��εǸ� �׽�Ʈ�� ������ �� �ִ�.
	if( pclsInfo->m_bLogin && m_bCallerLogin && m_bCalleeLogin )
	{
		if( m_bTest == false )
		{
			m_btnStartTest.EnableWindow( TRUE );
		}
	}
}

/**
 * @ingroup SipTest
 * @brief SIP ��ȭ ��û ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId	SIP Call-ID
 * @param pszFrom		SIP From ����� ���̵�
 * @param pszTo			SIP To ����� ���̵�
 * @param pclsRtp		RTP ���� ���� ��ü
 */
void CSipTestDlg::EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp )
{
	if( pclsRtp == NULL )
	{
		SetLog( "[ERROR] EventIncomingCall CSipCallRtp is NULL" );
		OnBnClickedStopTest();
		return;
	}

	if( gclsTestInfo.m_eTestType == E_TEST_CANCEL ) 
	{
		return;
	}
	else if( gclsTestInfo.m_eTestType == E_TEST_DECLINE ) 
	{
		gclsSipUserAgent.StopCall( pszCallId, SIP_DECLINE );
		return;
	}
	else if( gclsTestInfo.m_eTestType == E_TEST_TRANSFER_CALL )
	{
		CSipCallRtp clsRtp;

		clsRtp.m_strIp = gclsSipUserAgent.m_clsSipStack.m_clsSetup.m_strLocalIp;
		clsRtp.m_iPort = gclsTestInfo.m_clsCallerRtp.m_iPort;	
		clsRtp.m_iCodec = 0;

		// blind transfer ����� ���ŵǴ� INVITE �� caller #2 ���� ���ŵȴ�.
		// RTP ���� �׽�Ʈ�� �����ϰ� �ϱ� ���ؼ� caller #2 ������ caller #1 ���� ��ü�Ѵ�.
		gclsTestInfo.m_clsCallerPeerRtp = *pclsRtp;
		gclsTestInfo.m_strCallerCallId = pszCallId;

		gclsSipUserAgent.AcceptCall( pszCallId, &clsRtp );

		// freeSwitch ������ blind transfer �� callee �� ReINVITE �޽����� ���۵��� �ʴ´�.
		StartRtpThread();

		return;
	}

	CSipCallRtp clsRtp;

	clsRtp.m_strIp = gclsSipUserAgent.m_clsSipStack.m_clsSetup.m_strLocalIp;
	clsRtp.m_iPort = gclsTestInfo.m_clsCalleeRtp.m_iPort;	
	clsRtp.m_iCodec = 0;

#ifdef USE_MEDIA_LIST
	if( gclsSetup.m_bUseTwoMedia )
	{
		CSdpMedia sdpAudio( "audio", gclsTestInfo.m_clsCalleeRtp.m_iPort, "RTP/AVP" );
		CSdpMedia sdpVideo( "video", gclsTestInfo.m_clsCalleeVideoRtp.m_iPort, "RTP/AVP" );

		sdpAudio.AddAttribute( "rtpmap", "0 PCMU/8000" );
		
		sdpVideo.AddAttribute( "rtpmap", "97 H264/90000" );
		sdpVideo.AddAttribute( "fmtp", "97 profile-level-id=42" );

		clsRtp.m_clsMediaList.push_back( sdpAudio );
		clsRtp.m_clsMediaList.push_back( sdpVideo );
	}
#endif

	gclsTestInfo.m_clsCalleePeerRtp = *pclsRtp;
	gclsTestInfo.m_strCalleeCallId = pszCallId;

	gclsSipUserAgent.AcceptCall( pszCallId, &clsRtp );
}

/**
 * @ingroup SipTest
 * @brief SIP Ring / Session Progress ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId		SIP Call-ID
 * @param iSipStatus	SIP ���� �ڵ�
 * @param pclsRtp			RTP ���� ���� ��ü
 */
void CSipTestDlg::EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp )
{
	if( gclsTestInfo.m_eTestType == E_TEST_CANCEL )
	{
		gclsSipUserAgent.StopCall( pszCallId );
	}
}

/**
 * @ingroup SipTest
 * @brief SIP ��ȭ ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId	SIP Call-ID
 * @param pclsRtp		RTP ���� ���� ��ü
 */
void CSipTestDlg::EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	if( pclsRtp == NULL )
	{
		SetLog( "[ERROR] EventCallStart CSipCallRtp is NULL" );
		OnBnClickedStopTest();
		return;
	}

	if( gclsTestInfo.m_eTestType == E_TEST_TRANSFER_CALL )
	{
		gclsTestInfo.m_bRtpThreadEnd = true;
		return;
	}
	else if( gclsTestInfo.m_eTestType == E_TEST_DECLINE )
	{
		// freeSwitch �� ������ ���� INVITE �� ���� 200 OK ������ ���ŵȴ�.
		gclsSipUserAgent.StopCall( pszCallId );
		return;
	}

	gclsTestInfo.m_clsCallerPeerRtp = *pclsRtp;

	StartRtpThread();
}

/**
 * @ingroup SipTest
 * @brief SIP ��ȭ ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId		SIP Call-ID
 * @param iSipStatus	SIP ���� �ڵ�. INVITE �� ���� ���� �������� ��ȭ�� ����� ���, INVITE �� ���� �ڵ带 �����Ѵ�.
 */
void CSipTestDlg::EventCallEnd( const char * pszCallId, int iSipStatus )
{
}

/**
 * @ingroup SipTest
 * @brief SIP ReINVITE ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId	SIP Call-ID
 * @param pclsRemoteRtp		���� RTP ���� ���� ��ü
 * @param pclsLocalRtp		�� RTP ���� ���� ��ü
 */
void CSipTestDlg::EventReInvite( const char * pszCallId, CSipCallRtp * pclsRemoteRtp, CSipCallRtp * pclsLocalRtp )
{
	if( gclsTestInfo.m_eTestType == E_TEST_TRANSFER_CALL ) 
	{
		// blind transfer ����� ���ŵǴ� ReINVITE �� callee ���� ���ŵȴ�.
		gclsTestInfo.m_clsCalleePeerRtp = *pclsRemoteRtp;
		StartRtpThread();
	}
	else if( gclsTestInfo.m_eTestType == E_TEST_SCREENED_TRANSFER_CALL )
	{
		if( !strcmp( gclsTestInfo.m_strCallerCallId.c_str(), pszCallId ) )
		{
			gclsTestInfo.m_clsCallerPeerRtp = *pclsRemoteRtp;
		}
		else if( !strcmp( gclsTestInfo.m_strCalleeCallId.c_str(), pszCallId ) )
		{
			gclsTestInfo.m_clsCalleePeerRtp = *pclsRemoteRtp;
			StartRtpThread();
		}
	}
}

/**
 * @ingroup SipTest
 * @brief Screened / Unscreened Transfer ��û ���� �̺�Ʈ �ڵ鷯
 * @param pszCallId					SIP Call-ID
 * @param pszReferToCallId	��ȭ�� ���޵� SIP Call-ID
 * @param bScreenedTransfer Screened Transfer �̸� true �� �Էµǰ� Unscreened Transfer �̸� false �� �Էµȴ�.
 * @returns ��û�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipTestDlg::EventTransfer( const char * pszCallId, const char * pszReferToCallId, bool bScreenedTransfer )
{
	return false;
}

/**
 * @ingroup SipTest
 * @brief Blind Transfer ��û ���� �̺�Ʈ �ڵ鷯
 * @param pszCallId			SIP Call-ID
 * @param pszReferToId	��ȭ�� ���޵� ����� ���̵�
 * @returns ��û�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipTestDlg::EventBlindTransfer( const char * pszCallId, const char * pszReferToId )
{
	return false;
}

/**
 * @ingroup SipTest
 * @brief SIP ��ȭ ���� ���� ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId		SIP Call-ID
 * @param iSipStatus	SIP ���� �ڵ�.
 */
void CSipTestDlg::EventTransferResponse( const char * pszCallId, int iSipStatus )
{
	if( iSipStatus >= 300 )
	{
		gclsTestInfo.m_eTransferResult = E_TR_ERROR;
	}
	else if( iSipStatus == SIP_OK )
	{
		gclsTestInfo.m_eTransferResult = E_TR_SUCCESS;
	}
}

/**
 * @ingroup SipTest
 * @brief SIP MESSAGE ���� �̺�Ʈ �ڵ鷯
 * @param pszFrom			SIP �޽��� ���� ���̵�
 * @param pszTo				SIP �޽��� ���� ���̵�
 * @param pclsMessage SIP �޽���
 * @returns ��û�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipTestDlg::EventMessage( const char * pszFrom, const char * pszTo, CSipMessage * pclsMessage )
{
	return false;
}

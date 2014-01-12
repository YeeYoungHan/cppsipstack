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
 * @brief SIP REGISTER 응답 메시지 수신 이벤트 핸들러
 * @param pclsInfo	SIP REGISTER 응답 메시지를 전송한 IP-PBX 정보 저장 객체
 * @param iStatus		SIP REGISTER 응답 코드
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

	// Caller, Callee 가 모두 로그인되면 테스트를 시작할 수 있다.
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
 * @brief SIP 통화 요청 수신에 대한 인증 확인 이벤트 핸들러
 * @param pclsMessage	SIP INVITE 요청 메시지
 * @return 인증에 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipTestDlg::EventIncomingRequestAuth( CSipMessage * pclsMessage )
{
	return true;
}

/**
 * @ingroup SipTest
 * @brief SIP 통화 요청 수신 이벤트 핸들러
 * @param	pszCallId	SIP Call-ID
 * @param pszFrom		SIP From 사용자 아이디
 * @param pszTo			SIP To 사용자 아이디
 * @param pclsRtp		RTP 정보 저장 객체
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

	CSipCallRtp clsRtp;

	clsRtp.m_strIp = gclsSipUserAgent.m_clsSipStack.m_clsSetup.m_strLocalIp;
	clsRtp.m_iPort = gclsTestInfo.m_clsCalleeRtp.m_iPort;	
	clsRtp.m_iCodec = 0;

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

	gclsTestInfo.m_clsCalleePeerRtp = *pclsRtp;
	gclsTestInfo.m_strCalleeCallId = pszCallId;

	gclsSipUserAgent.AcceptCall( pszCallId, &clsRtp );
}

/**
 * @ingroup SipTest
 * @brief SIP Ring / Session Progress 수신 이벤트 핸들러
 * @param	pszCallId		SIP Call-ID
 * @param iSipStatus	SIP 응답 코드
 * @param pclsRtp			RTP 정보 저장 객체
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
 * @brief SIP 통화 연결 이벤트 핸들러
 * @param	pszCallId	SIP Call-ID
 * @param pclsRtp		RTP 정보 저장 객체
 */
void CSipTestDlg::EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	if( pclsRtp == NULL )
	{
		SetLog( "[ERROR] EventCallStart CSipCallRtp is NULL" );
		OnBnClickedStopTest();
		return;
	}

	gclsTestInfo.m_clsCallerPeerRtp = *pclsRtp;

	StartRtpThread();
}

/**
 * @ingroup SipTest
 * @brief SIP 통화 종료 이벤트 핸들러
 * @param	pszCallId		SIP Call-ID
 * @param iSipStatus	SIP 응답 코드. INVITE 에 대한 오류 응답으로 전화가 종료된 경우, INVITE 의 응답 코드를 저장한다.
 */
void CSipTestDlg::EventCallEnd( const char * pszCallId, int iSipStatus )
{
}

/**
 * @ingroup SipTest
 * @brief SIP ReINVITE 수신 이벤트 핸들러
 * @param	pszCallId	SIP Call-ID
 * @param pclsRtp		RTP 정보 저장 객체
 */
void CSipTestDlg::EventReInvite( const char * pszCallId, CSipCallRtp * pclsRtp )
{
}

/**
 * @ingroup SipTest
 * @brief Screened / Unscreened Transfer 요청 수신 이벤트 핸들러
 * @param pszCallId					SIP Call-ID
 * @param pszReferToCallId	전화가 전달될 SIP Call-ID
 * @param bScreenedTransfer Screened Transfer 이면 true 가 입력되고 Unscreened Transfer 이면 false 가 입력된다.
 * @returns 요청을 수락하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipTestDlg::EventTransfer( const char * pszCallId, const char * pszReferToCallId, bool bScreenedTransfer )
{
	return false;
}

/**
 * @ingroup SipTest
 * @brief Blind Transfer 요청 수신 이벤트 핸들러
 * @param pszCallId			SIP Call-ID
 * @param pszReferToId	전화가 전달될 사용자 아이디
 * @returns 요청을 수락하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipTestDlg::EventBlindTransfer( const char * pszCallId, const char * pszReferToId )
{
	return false;
}

/**
 * @ingroup SipTest
 * @brief SIP MESSAGE 수신 이벤트 핸들러
 * @param pszFrom			SIP 메시지 전송 아이디
 * @param pszTo				SIP 메시지 수신 아이디
 * @param pclsMessage SIP 메시지
 * @returns 요청을 수락하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipTestDlg::EventMessage( const char * pszFrom, const char * pszTo, CSipMessage * pclsMessage )
{
	return false;
}

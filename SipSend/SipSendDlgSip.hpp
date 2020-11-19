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

LRESULT CSipSendDlg::OnSipMessage( WPARAM wParam, LPARAM lParam )
{
	
	return 0;
}

/**
 * @ingroup SipSpeed
 * @brief SIP REGISTER 응답 메시지 수신 이벤트 핸들러
 * @param pclsInfo	SIP REGISTER 응답 메시지를 전송한 IP-PBX 정보 저장 객체
 * @param iStatus		SIP REGISTER 응답 코드
 */
void CSipSendDlg::EventRegister( CSipServerInfo * pclsInfo, int iStatus )
{
}

/**
 * @ingroup SipSpeed
 * @brief SIP 통화 요청 수신 이벤트 핸들러
 * @param	pszCallId	SIP Call-ID
 * @param pszFrom		SIP From 사용자 아이디
 * @param pszTo			SIP To 사용자 아이디
 * @param pclsRtp		RTP 정보 저장 객체
 */
void CSipSendDlg::EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp )
{
	gclsSipUserAgent.StopCall( pszCallId, SIP_BUSY_HERE );
}

/**
 * @ingroup SipSpeed
 * @brief SIP Ring / Session Progress 수신 이벤트 핸들러
 * @param	pszCallId		SIP Call-ID
 * @param iSipStatus	SIP 응답 코드
 * @param pclsRtp			RTP 정보 저장 객체
 */
void CSipSendDlg::EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp )
{
}

/**
 * @ingroup SipSpeed
 * @brief SIP 통화 연결 이벤트 핸들러
 * @param	pszCallId	SIP Call-ID
 * @param pclsRtp		RTP 정보 저장 객체
 */
void CSipSendDlg::EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp )
{

}

/**
 * @ingroup SipSpeed
 * @brief SIP 통화 종료 이벤트 핸들러
 * @param	pszCallId		SIP Call-ID
 * @param iSipStatus	SIP 응답 코드. INVITE 에 대한 오류 응답으로 전화가 종료된 경우, INVITE 의 응답 코드를 저장한다.
 */
void CSipSendDlg::EventCallEnd( const char * pszCallId, int iSipStatus )
{
	
}

/**
 * @ingroup SipSpeed
 * @brief SIP 요청 메시지 수신 이벤트 핸들러
 * @param iThreadId		UDP 쓰레드 번호
 * @param pclsMessage SIP 요청 메시지
 * @returns SIP 요청 메시지를 처리하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipSendDlg::RecvRequest( int iThreadId, CSipMessage * pclsMessage )
{
	return false;
}

/**
 * @ingroup SipSpeed
 * @brief SIP 응답 메시지 수신 이벤트 핸들러
 * @param iThreadId		UDP 쓰레드 번호
 * @param pclsMessage SIP 응답 메시지
 * @returns SIP 응답 메시지를 처리하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipSendDlg::RecvResponse( int iThreadId, CSipMessage * pclsMessage )
{
	return false;
}

/**
 * @ingroup SipSpeed
 * @brief SIP 메시지 전송 timeout 이벤트 핸들러
 * @param iThreadId		UDP 쓰레드 번호
 * @param pclsMessage SIP 응답 메시지
 * @returns SIP 응답 메시지를 처리하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipSendDlg::SendTimeout( int iThreadId, CSipMessage * pclsMessage )
{
	return false;
}

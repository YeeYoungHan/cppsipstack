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

package com.cppsipstack;

/** SipUserAgent 의 이벤트 수신용 callback interface
 * @ingroup SipUserAgentAndroid
 * @author websearch (websearch@naver.com)
 */
public interface SipUserAgentCallBack
{
	/** SIP REGISTER 응답 메시지 수신 이벤트 핸들러
	 * @param clsInfo	SIP REGISTER 응답 메시지를 전송한 IP-PBX 정보 저장 객체
	 * @param iStatus SIP REGISTER 응답 코드
	 */
	public void EventRegister( SipServerInfo clsInfo, int iStatus );
	
	/** SIP 통화 요청 수신 이벤트 핸들러
	 * @param strCallId	SIP Call-ID
	 * @param strFrom		SIP From 사용자 아이디
	 * @param strTo			SIP To 사용자 아이디
	 * @param clsRtp		RTP 정보 저장 객체
	 */
	public void EventIncomingCall( String strCallId, String strFrom, String strTo, SipCallRtp clsRtp );
	
	/** SIP Ring / Session Progress 수신 이벤트 핸들러
	 * @param strCallId		SIP Call-ID
	 * @param iSipStatus	SIP 응답 코드
	 * @param clsRtp			RTP 정보 저장 객체
	 */
	public void EventCallRing( String strCallId, int iSipStatus, SipCallRtp clsRtp );
	
	/** SIP 통화 연결 이벤트 핸들러
	 * @param strCallId	SIP Call-ID
	 * @param clsRtp		RTP 정보 저장 객체
	 */
	public void EventCallStart( String strCallId, SipCallRtp clsRtp );
	
	/** SIP 통화 종료 이벤트 핸들러
	 * @param strCallId		SIP Call-ID
	 * @param iSipStatus	SIP 응답 코드. INVITE 에 대한 오류 응답으로 전화가 종료된 경우, INVITE 의 응답 코드를 저장한다.
	 */
	public void EventCallEnd( String strCallId, int iSipStatus );
	
	/** SIP ReINVITE 수신 이벤트 핸들러
	 * @param strCallId	SIP Call-ID
	 * @param clsRtp		RTP 정보 저장 객체
	 */
	public void EventReInvite( String strCallId, SipCallRtp clsRtp );
	
	/** Screened / Unscreened Transfer 요청 수신 이벤트 핸들러
	 * @param strCallId					SIP Call-ID
	 * @param strReferToCallId	전화가 전달될 SIP Call-ID
	 * @param bScreenedTransfer	Screened Transfer 이면 true 가 입력되고 Unscreened Transfer 이면 false 가 입력된다.
	 */
	public void EventTransfer( String strCallId, String strReferToCallId, boolean bScreenedTransfer );
	
	/** Blind Transfer 요청 수신 이벤트 핸들러
	 * @param strCallId			SIP Call-ID
	 * @param strReferToId	통화 전환될 상대방 아이디
	 */
	public void EventBlindTransfer( String strCallId, String strReferToId );
	
	/** SMS 요청 수신 이벤트 핸들러
	 * @param strFrom		SIP From 사용자 아이디
	 * @param strTo			SIP To 사용자 아이디
	 * @param strSms		SMS 내용
	 */
	public void EventMessage( String strFrom, String strTo, String strSms );
}

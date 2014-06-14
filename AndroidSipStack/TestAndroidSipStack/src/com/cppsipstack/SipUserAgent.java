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

/**
 * @defgroup SipUserAgentAndroid SipUserAgentAndroid
 * 안드로이드용 SIP User Agent 라이브러리
 */

/** NDK 와 연동하는 SIP User Agent 클래스
 * @ingroup SipUserAgentAndroid
 * @author websearch (websearch@naver.com)
 */
public class SipUserAgent
{
	public static SipUserAgentCallBack m_clsCallBack;

	/** 로그인할 SIP 서버 정보를 저장한다.
	 * @param clsInfo	SIP 서버 정보 저장 객체
	 * @return 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
	 */
	public static native boolean InsertRegisterInfo( SipServerInfo clsInfo );

	/** SIP stack 을 시작한다.
	 * @param clsSetup SIP stack 설정 정보 저장 객체
	 * @return 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
	 */
	public static native boolean Start( SipStackSetup clsSetup );
	
	/** SIP stack 을 종료한다.
	 * @return 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
	 */
	public static native boolean Stop( );
	
	/** SIP 통화 요청 메시지를 전송한다.
	 * @param strFrom	발신자 아이디
	 * @param strTo		수신자 아이디
	 * @param clsRtp	local RTP 정보 저장 객체
	 * @return 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
	 */
	public static native String StartCall( String strFrom, String strTo, SipCallRtp clsRtp );
	
	/** 통화를 종료한다. 
   *  통화 요청을 보내고 연결되지 않으면 통화 취소 메시지를 전송한다.
   *  통화 연결되었으면 통화 종료 메시지를 전송한다.
   *  통화 수락인 경우 통화 거절 응답 메시지를 전송한다.
	 * @param strCallId	SIP Call-ID
	 * @param iSipCode	통화 요청을 거절할 때에 전송할 SIP status code
	 * @return 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
	 */
	public static native boolean StopCall( String strCallId, int iSipCode );
	
	/** 수신된 통화를 수락한다.
	 * @param strCallId	SIP Call-ID
	 * @param clsRtp		local RTP 정보 저장 객체
	 * @return 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
	 */
	public static native boolean AcceptCall( String strCallId, SipCallRtp clsRtp );
	
	/** SipUserAgent 의 이벤트를 수신할 callback interface 를 등록한다.
	 * @param clsCallBack	callback interface
	 */
	public static void SetCallBack( SipUserAgentCallBack clsCallBack )
	{
		m_clsCallBack = clsCallBack;
	}
	
	/** SIP REGISTER 응답 메시지 수신 이벤트 핸들러
	 * @param clsInfo	SIP REGISTER 응답 메시지를 전송한 IP-PBX 정보 저장 객체
	 * @param iStatus SIP REGISTER 응답 코드
	 */
	public static void EventRegister( SipServerInfo clsInfo, int iStatus )
	{
		SipLog.Debug( "EventRegister sipserver(" + clsInfo.m_strIp + ":" + clsInfo.m_iPort + ") userid(" + 
				clsInfo.m_strUserId + ") status(" + iStatus + ")" );
		
		if( m_clsCallBack != null )
		{
			m_clsCallBack.EventRegister( clsInfo, iStatus );
		}
	}
	
	/** SIP 통화 요청 수신 이벤트 핸들러
	 * @param strCallId	SIP Call-ID
	 * @param strFrom		SIP From 사용자 아이디
	 * @param strTo			SIP To 사용자 아이디
	 * @param clsRtp		RTP 정보 저장 객체
	 */
	public static void EventIncomingCall( String strCallId, String strFrom, String strTo, SipCallRtp clsRtp )
	{
		SipLog.Debug( "EventIncomingCall callid(" + strCallId + ") from(" + strFrom + ") to(" + strTo + ") rtp("
				+ clsRtp.m_strIp + ":" + clsRtp.m_iPort + ") codec(" + clsRtp.m_iCodec + ") direction(" + clsRtp.m_iDirection + ")" );
		
		if( m_clsCallBack != null )
		{
			m_clsCallBack.EventIncomingCall( strCallId, strFrom, strTo, clsRtp );
		}
	}
	
	/** SIP Ring / Session Progress 수신 이벤트 핸들러
	 * @param strCallId		SIP Call-ID
	 * @param iSipStatus	SIP 응답 코드
	 * @param clsRtp			RTP 정보 저장 객체
	 */
	public static void EventCallRing( String strCallId, int iSipStatus, SipCallRtp clsRtp )
	{
		SipLog.Debug( "EventCallRing callid(" + strCallId + ") status(" + iSipStatus + ") rtp("
				+ clsRtp.m_strIp + ":" + clsRtp.m_iPort + ") codec(" + clsRtp.m_iCodec + ") direction(" + clsRtp.m_iDirection + ")" );
		
		if( m_clsCallBack != null )
		{
			m_clsCallBack.EventCallRing( strCallId, iSipStatus, clsRtp );
		}
	}
	
	/** SIP 통화 연결 이벤트 핸들러
	 * @param strCallId	SIP Call-ID
	 * @param clsRtp		RTP 정보 저장 객체
	 */
	public static void EventCallStart( String strCallId, SipCallRtp clsRtp )
	{
		SipLog.Debug( "EventCallStart callid(" + strCallId + ") rtp("
				+ clsRtp.m_strIp + ":" + clsRtp.m_iPort + ") codec(" + clsRtp.m_iCodec + ") direction(" + clsRtp.m_iDirection + ")" );
		
		if( m_clsCallBack != null )
		{
			m_clsCallBack.EventCallStart( strCallId, clsRtp );
		}
	}
	
	/** SIP 통화 종료 이벤트 핸들러
	 * @param strCallId		SIP Call-ID
	 * @param iSipStatus	SIP 응답 코드. INVITE 에 대한 오류 응답으로 전화가 종료된 경우, INVITE 의 응답 코드를 저장한다.
	 */
	public static void EventCallEnd( String strCallId, int iSipStatus )
	{
		SipLog.Debug( "EventCallEnd callid(" + strCallId + ") status(" + iSipStatus + ")" );
				
		if( m_clsCallBack != null )
		{
			m_clsCallBack.EventCallEnd( strCallId, iSipStatus );
		}
	}
	
	/** SIP ReINVITE 수신 이벤트 핸들러
	 * @param strCallId	SIP Call-ID
	 * @param clsRtp		RTP 정보 저장 객체
	 */
	public static void EventReInvite( String strCallId, SipCallRtp clsRtp )
	{
		SipLog.Debug( "EventReInvite callid(" + strCallId + ") rtp("
				+ clsRtp.m_strIp + ":" + clsRtp.m_iPort + ") codec(" + clsRtp.m_iCodec + ") direction(" + clsRtp.m_iDirection + ")" );
		
		if( m_clsCallBack != null )
		{
			m_clsCallBack.EventReInvite( strCallId, clsRtp );
		}
	}
	
	/** Screened / Unscreened Transfer 요청 수신 이벤트 핸들러
	 * @param strCallId					SIP Call-ID
	 * @param strReferToCallId	전화가 전달될 SIP Call-ID
	 * @param bScreenedTransfer	Screened Transfer 이면 true 가 입력되고 Unscreened Transfer 이면 false 가 입력된다.
	 */
	public static void EventTransfer( String strCallId, String strReferToCallId, boolean bScreenedTransfer )
	{
		SipLog.Debug( "EventBlindTransfer callid(" + strCallId + ") refertocallid(" + strReferToCallId + ") screened("
				+ ( bScreenedTransfer ? "true" : "false" ) + ")" );
		
		if( m_clsCallBack != null )
		{
			m_clsCallBack.EventTransfer( strCallId, strReferToCallId, bScreenedTransfer );
		}
	}
	
	/** Blind Transfer 요청 수신 이벤트 핸들러
	 * @param strCallId			SIP Call-ID
	 * @param strReferToId	통화 전환될 상대방 아이디
	 */
	public static void EventBlindTransfer( String strCallId, String strReferToId )
	{
		SipLog.Debug( "EventBlindTransfer callid(" + strCallId + ") refertoid(" + strReferToId + ")" );
		
		if( m_clsCallBack != null )
		{
			m_clsCallBack.EventBlindTransfer( strCallId, strReferToId );
		}
	}
	
	/** SMS 요청 수신 이벤트 핸들러
	 * @param strFrom		SIP From 사용자 아이디
	 * @param strTo			SIP To 사용자 아이디
	 * @param strSms		SMS 내용
	 */
	public static void EventMessage( String strFrom, String strTo, String strSms )
	{
		SipLog.Debug( "EventMessage from(" + strFrom + ") to(" + strTo + ") sms(" + strSms + ")" );
		
		if( m_clsCallBack != null )
		{
			m_clsCallBack.EventMessage( strFrom, strTo, strSms );
		}
	}
	
	static
	{
		SipLog.Debug( "load AndroidSipStack" );
		
		try
		{
			System.loadLibrary( "CppSipStack" );
		}
		catch( Throwable e )
		{
			SipLog.Error( "loadLibrary error - " + e.toString( ) );
		}
  }
}

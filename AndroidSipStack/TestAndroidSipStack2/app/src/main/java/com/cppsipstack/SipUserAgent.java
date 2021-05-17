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
 * �ȵ���̵�� SIP User Agent ���̺귯��
 */

/** NDK �� �����ϴ� SIP User Agent Ŭ����
 * @ingroup SipUserAgentAndroid
 * @author websearch (websearch@naver.com)
 */
public class SipUserAgent
{
	public static SipUserAgentCallBack m_clsCallBack;

	/** �α����� SIP ���� ������ �����Ѵ�.
	 * @param clsInfo	SIP ���� ���� ���� ��ü
	 * @return �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
	 */
	public static native boolean InsertRegisterInfo( SipServerInfo clsInfo );

	/** SIP stack �� �����Ѵ�.
	 * @param clsSetup SIP stack ���� ���� ���� ��ü
	 * @return �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
	 */
	public static native boolean Start( SipStackSetup clsSetup );
	
	/** SIP stack �� �����Ѵ�.
	 * @return �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
	 */
	public static native boolean Stop( );
	
	/** SIP ��ȭ ��û �޽����� �����Ѵ�.
	 * @param strFrom	�߽��� ���̵�
	 * @param strTo		������ ���̵�
	 * @param clsRtp	local RTP ���� ���� ��ü
	 * @return �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
	 */
	public static native String StartCall( String strFrom, String strTo, SipCallRtp clsRtp );
	
	/** ��ȭ�� �����Ѵ�. 
   *  ��ȭ ��û�� ������ ������� ������ ��ȭ ��� �޽����� �����Ѵ�.
   *  ��ȭ ����Ǿ����� ��ȭ ���� �޽����� �����Ѵ�.
   *  ��ȭ ������ ��� ��ȭ ���� ���� �޽����� �����Ѵ�.
	 * @param strCallId	SIP Call-ID
	 * @param iSipCode	��ȭ ��û�� ������ ���� ������ SIP status code
	 * @return �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
	 */
	public static native boolean StopCall( String strCallId, int iSipCode );
	
	/** ���ŵ� ��ȭ�� �����Ѵ�.
	 * @param strCallId	SIP Call-ID
	 * @param clsRtp		local RTP ���� ���� ��ü
	 * @return �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
	 */
	public static native boolean AcceptCall( String strCallId, SipCallRtp clsRtp );
	
	/** Blind transfer �� �����Ѵ�.
	 * @param strCallId SIP Call-ID
	 * @param strTo			��ȭ ������ ���� ���̵�
	 * @return �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
	 */
	public static native boolean TransferCallBlind( String strCallId, String strTo );
	
	/** Screened / Unscreened transfer �� �����Ѵ�.
	 * @param strCallId		SIP Call-ID
	 * @param strToCallId	��ȭ ������ ���� SIP Call-ID
	 * @return �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
	 */
	public static native boolean TransferCall( String strCallId, String strToCallId );
	
	/** ReINVITE �޽����� �����Ѵ�.
	 * @param strCallId	SIP Call-ID
	 * @param clsRtp		local RTP ���� ���� ��ü
	 * @return �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
	 */
	public static native boolean SendReInvite( String strCallId, SipCallRtp clsRtp );
	
	/** SMS �޽����� �����Ѵ�.
	 * @param strFrom	�߽��� ���̵�
	 * @param strTo		������ ���̵�
	 * @param strText	�޽���
	 * @return �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
	 */
	public static native boolean SendSms( String strFrom, String strTo, String strText );
	
	/** SipUserAgent �� �̺�Ʈ�� ������ callback interface �� ����Ѵ�.
	 * @param clsCallBack	callback interface
	 */
	public static void SetCallBack( SipUserAgentCallBack clsCallBack )
	{
		m_clsCallBack = clsCallBack;
	}
	
	/** SIP REGISTER ���� �޽��� ���� �̺�Ʈ �ڵ鷯
	 * @param clsInfo	SIP REGISTER ���� �޽����� ������ IP-PBX ���� ���� ��ü
	 * @param iStatus SIP REGISTER ���� �ڵ�
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
	
	/** SIP ��ȭ ��û ���� �̺�Ʈ �ڵ鷯
	 * @param strCallId	SIP Call-ID
	 * @param strFrom		SIP From ����� ���̵�
	 * @param strTo			SIP To ����� ���̵�
	 * @param clsRtp		RTP ���� ���� ��ü
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
	
	/** SIP Ring / Session Progress ���� �̺�Ʈ �ڵ鷯
	 * @param strCallId		SIP Call-ID
	 * @param iSipStatus	SIP ���� �ڵ�
	 * @param clsRtp			RTP ���� ���� ��ü
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
	
	/** SIP ��ȭ ���� �̺�Ʈ �ڵ鷯
	 * @param strCallId	SIP Call-ID
	 * @param clsRtp		RTP ���� ���� ��ü
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
	
	/** SIP ��ȭ ���� �̺�Ʈ �ڵ鷯
	 * @param strCallId		SIP Call-ID
	 * @param iSipStatus	SIP ���� �ڵ�. INVITE �� ���� ���� �������� ��ȭ�� ����� ���, INVITE �� ���� �ڵ带 �����Ѵ�.
	 */
	public static void EventCallEnd( String strCallId, int iSipStatus )
	{
		SipLog.Debug( "EventCallEnd callid(" + strCallId + ") status(" + iSipStatus + ")" );
				
		if( m_clsCallBack != null )
		{
			m_clsCallBack.EventCallEnd( strCallId, iSipStatus );
		}
	}
	
	/** SIP ReINVITE ���� �̺�Ʈ �ڵ鷯
	 * @param strCallId	SIP Call-ID
	 * @param clsRemoteRtp	remote RTP ���� ���� ��ü
	 * @param clsLocalRtp		local RTP ���� ���� ��ü
	 */
	public static void EventReInvite( String strCallId, SipCallRtp clsRemoteRtp, SipCallRtp clsLocalRtp )
	{
		SipLog.Debug( "EventReInvite callid(" + strCallId + ") rtp("
				+ clsRemoteRtp.m_strIp + ":" + clsRemoteRtp.m_iPort + ") codec(" + clsRemoteRtp.m_iCodec + ") direction(" + clsRemoteRtp.m_iDirection + ")" );
		
		if( m_clsCallBack != null )
		{
			m_clsCallBack.EventReInvite( strCallId, clsRemoteRtp, clsLocalRtp );
		}
	}
	
	/** SIP PRACK ���� �̺�Ʈ �ڵ鷯
	 * @param strCallId SIP Call-ID
	 * @param clsRtp		���� RTP ���� ���� ��ü
	 */
	public static void EventPrack( String strCallId, SipCallRtp clsRtp )
	{
		SipLog.Debug( "EventPrack callid(" + strCallId + ") rtp("
				+ clsRtp.m_strIp + ":" + clsRtp.m_iPort + ") codec(" + clsRtp.m_iCodec + ") direction(" + clsRtp.m_iDirection + ")" );
		
		if( m_clsCallBack != null )
		{
			m_clsCallBack.EventPrack( strCallId, clsRtp );
		}
	}
	
	/** Screened / Unscreened Transfer ��û ���� �̺�Ʈ �ڵ鷯
	 * @param strCallId					SIP Call-ID
	 * @param strReferToCallId	��ȭ�� ���޵� SIP Call-ID
	 * @param bScreenedTransfer	Screened Transfer �̸� true �� �Էµǰ� Unscreened Transfer �̸� false �� �Էµȴ�.
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
	
	/** Blind Transfer ��û ���� �̺�Ʈ �ڵ鷯
	 * @param strCallId			SIP Call-ID
	 * @param strReferToId	��ȭ ��ȯ�� ���� ���̵�
	 */
	public static void EventBlindTransfer( String strCallId, String strReferToId )
	{
		SipLog.Debug( "EventBlindTransfer callid(" + strCallId + ") refertoid(" + strReferToId + ")" );
		
		if( m_clsCallBack != null )
		{
			m_clsCallBack.EventBlindTransfer( strCallId, strReferToId );
		}
	}
	
	/** SMS ��û ���� �̺�Ʈ �ڵ鷯
	 * @param strFrom		SIP From ����� ���̵�
	 * @param strTo			SIP To ����� ���̵�
	 * @param strSms		SMS ����
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

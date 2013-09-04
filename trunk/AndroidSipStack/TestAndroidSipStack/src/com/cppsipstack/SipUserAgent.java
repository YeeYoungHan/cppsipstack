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
 * @author websearch (websearch@naver.com)
 *
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
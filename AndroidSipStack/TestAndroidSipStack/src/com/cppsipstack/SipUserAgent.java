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

public class SipUserAgent
{
	public static SipUserAgentCallBack m_clsCallBack;
	
	public static native boolean InsertRegisterInfo( SipServerInfo clsInfo );
	public static native boolean Start( SipStackSetup clsSetup );

	public static void SetCallBack( SipUserAgentCallBack clsCallBack )
	{
		m_clsCallBack = clsCallBack;
	}
	
	public static void EventRegister( SipServerInfo clsInfo, int iStatus )
	{
		SipLog.Debug( "EventRegister sipserver(" + clsInfo.m_strIp + ":" + clsInfo.m_iPort + ") userid(" + 
				clsInfo.m_strUserId + ") status(" + iStatus + ")" );
		
		if( m_clsCallBack != null )
		{
			m_clsCallBack.EventRegister( clsInfo, iStatus );
		}
	}
	
	public static void EventIncomingCall( String strCallId, String strFrom, String strTo, SipCallRtp clsRtp )
	{
		SipLog.Debug( "EventIncomingCall callid(" + strCallId + ") from(" + strFrom + ") to(" + strTo + ") rtp("
				+ clsRtp.m_strIp + ":" + clsRtp.m_iPort + ") codec(" + clsRtp.m_iCodec + ") direction(" + clsRtp.m_iDirection + ")" );
		
		if( m_clsCallBack != null )
		{
			m_clsCallBack.EventIncomingCall( strCallId, strFrom, strTo, clsRtp );
		}
	}
	
	public static void EventCallRing( String strCallId, int iSipStatus, SipCallRtp clsRtp )
	{
		SipLog.Debug( "EventCallRing callid(" + strCallId + ") status(" + iSipStatus + ") rtp("
				+ clsRtp.m_strIp + ":" + clsRtp.m_iPort + ") codec(" + clsRtp.m_iCodec + ") direction(" + clsRtp.m_iDirection + ")" );
		
		if( m_clsCallBack != null )
		{
			m_clsCallBack.EventCallRing( strCallId, iSipStatus, clsRtp );
		}
	}
	
	public static void EventCallStart( String strCallId, SipCallRtp clsRtp )
	{
		SipLog.Debug( "EventCallStart callid(" + strCallId + ") rtp("
				+ clsRtp.m_strIp + ":" + clsRtp.m_iPort + ") codec(" + clsRtp.m_iCodec + ") direction(" + clsRtp.m_iDirection + ")" );
		
		if( m_clsCallBack != null )
		{
			m_clsCallBack.EventCallStart( strCallId, clsRtp );
		}
	}
	
	public static void EventCallEnd( String strCallId, int iSipStatus )
	{
		SipLog.Debug( "EventCallStart callid(" + strCallId + ") status(" + iSipStatus + ")" );
				
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

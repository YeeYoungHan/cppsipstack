package com.cppsipstack;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

public class SipUserAgentHandler extends Handler implements SipUserAgentCallBack
{
	SipUserAgentCallBack m_clsCallBack;
	
	public SipUserAgentHandler( SipUserAgentCallBack clsCallBack )
	{
		m_clsCallBack = clsCallBack;
	}
	
	public void handleMessage( Message m )
	{
		Bundle b = m.getData( );
		
		String strCommand = (String)b.get( "command" );
		
		if( strCommand.equals( "EventRegister" ) )
		{
			SipServerInfo clsInfo = (SipServerInfo)b.getSerializable( "clsInfo" );
			int iStatus = b.getInt( "iStatus" );
			
			m_clsCallBack.EventRegister( clsInfo, iStatus );
		}
		else if( strCommand.equals( "EventIncomingCall" ) )
		{
			String strCallId = b.getString( "strCallId" );
			String strFrom = b.getString( "strFrom" );
			String strTo = b.getString( "strTo" );
			SipCallRtp clsRtp = (SipCallRtp)b.getSerializable( "clsRtp" );
			
			m_clsCallBack.EventIncomingCall( strCallId, strFrom, strTo, clsRtp );
		}
		else if( strCommand.equals( "EventCallRing" ) )
		{
			String strCallId = b.getString( "strCallId" );
			int iSipStatus = b.getInt( "iSipStatus" );
			SipCallRtp clsRtp = (SipCallRtp)b.getSerializable( "clsRtp" );
			
			m_clsCallBack.EventCallRing( strCallId, iSipStatus, clsRtp );
		}
		else if( strCommand.equals( "EventCallStart" ) )
		{
			String strCallId = b.getString( "strCallId" );
			SipCallRtp clsRtp = (SipCallRtp)b.getSerializable( "clsRtp" );
			
			m_clsCallBack.EventCallStart( strCallId, clsRtp );
		}
		else if( strCommand.equals( "EventCallEnd" ) )
		{
			String strCallId = b.getString( "strCallId" );
			int iSipStatus = b.getInt( "iSipStatus" );
			
			m_clsCallBack.EventCallEnd( strCallId, iSipStatus );
		}
		else if( strCommand.equals( "EventReInvite" ) )
		{
			String strCallId = b.getString( "strCallId" );
			SipCallRtp clsRtp = (SipCallRtp)b.getSerializable( "clsRtp" );
			
			m_clsCallBack.EventReInvite( strCallId, clsRtp );
		}
	}

	@Override
	public void EventRegister( SipServerInfo clsInfo, int iStatus )
	{
		Bundle b = new Bundle();
  	
  	Message m = obtainMessage( );
  	b.putString( "command", "EventRegister" );
  	b.putSerializable( "clsInfo", clsInfo );
  	b.putInt( "iStatus", iStatus );
  	m.setData( b );
  	sendMessage( m );
	}

	@Override
	public void EventIncomingCall( String strCallId, String strFrom, String strTo, SipCallRtp clsRtp )
	{
		Bundle b = new Bundle();
  	
  	Message m = obtainMessage( );
  	b.putString( "command", "EventIncomingCall" );
  	b.putString( "strCallId", strCallId );
  	b.putString( "strFrom", strFrom );
  	b.putString( "strTo", strTo );
  	b.putSerializable( "clsRtp", clsRtp );
  	m.setData( b );
  	sendMessage( m );
	}

	@Override
	public void EventCallRing( String strCallId, int iSipStatus, SipCallRtp clsRtp )
	{
		Bundle b = new Bundle();
  	
  	Message m = obtainMessage( );
  	b.putString( "command", "EventCallRing" );
  	b.putString( "strCallId", strCallId );
  	b.putInt( "iSipStatus", iSipStatus );
  	b.putSerializable( "clsRtp", clsRtp );
  	m.setData( b );
  	sendMessage( m );
	}

	@Override
	public void EventCallStart( String strCallId, SipCallRtp clsRtp )
	{
		Bundle b = new Bundle();
  	
  	Message m = obtainMessage( );
  	b.putString( "command", "EventCallStart" );
  	b.putString( "strCallId", strCallId );
  	b.putSerializable( "clsRtp", clsRtp );
  	m.setData( b );
  	sendMessage( m );
	}

	@Override
	public void EventCallEnd( String strCallId, int iSipStatus )
	{
		Bundle b = new Bundle();
  	
  	Message m = obtainMessage( );
  	b.putString( "command", "EventCallEnd" );
  	b.putString( "strCallId", strCallId );
  	b.putInt( "iSipStatus", iSipStatus );
  	m.setData( b );
  	sendMessage( m );
	}

	@Override
	public void EventReInvite( String strCallId, SipCallRtp clsRtp )
	{
		Bundle b = new Bundle();
  	
  	Message m = obtainMessage( );
  	b.putString( "command", "EventReInvite" );
  	b.putString( "strCallId", strCallId );
  	b.putSerializable( "clsRtp", clsRtp );
  	m.setData( b );
  	sendMessage( m );
	}
}

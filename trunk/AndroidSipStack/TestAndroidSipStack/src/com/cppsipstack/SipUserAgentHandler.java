package com.cppsipstack;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

/** SipUserAgent callback ���� ȣ��� �̺�Ʈ�� GUI SipUserAgent callback ���� ȣ���ϴ� Ŭ����
 * @ingroup SipUserAgentAndroid
 * @author �̿��� ( http://blog.naver.com/websearch )
 */
public class SipUserAgentHandler extends Handler implements SipUserAgentCallBack, SipUserAgentHandlerDefine
{
	SipUserAgentCallBack m_clsCallBack;
	
	public SipUserAgentHandler( SipUserAgentCallBack clsCallBack )
	{
		m_clsCallBack = clsCallBack;
	}
	
	public void handleMessage( Message m )
	{
		Bundle b = m.getData( );
		
		String strCommand = (String)b.get( COMMAND );
		
		if( strCommand.equals( EVENT_REGISTER ) )
		{
			SipServerInfo clsInfo = (SipServerInfo)b.getSerializable( ARG_INFO );
			int iStatus = b.getInt( ARG_STATUS );
			
			m_clsCallBack.EventRegister( clsInfo, iStatus );
		}
		else if( strCommand.equals( EVENT_INCOMING_CALL ) )
		{
			String strCallId = b.getString( ARG_CALL_ID );
			String strFrom = b.getString( ARG_FROM );
			String strTo = b.getString( ARG_TO );
			SipCallRtp clsRtp = (SipCallRtp)b.getSerializable( ARG_RTP );
			
			m_clsCallBack.EventIncomingCall( strCallId, strFrom, strTo, clsRtp );
		}
		else if( strCommand.equals( EVENT_CALL_RING ) )
		{
			String strCallId = b.getString( ARG_CALL_ID );
			int iSipStatus = b.getInt( ARG_SIP_STATUS );
			SipCallRtp clsRtp = (SipCallRtp)b.getSerializable( ARG_RTP );
			
			m_clsCallBack.EventCallRing( strCallId, iSipStatus, clsRtp );
		}
		else if( strCommand.equals( EVENT_CALL_START ) )
		{
			String strCallId = b.getString( ARG_CALL_ID );
			SipCallRtp clsRtp = (SipCallRtp)b.getSerializable( ARG_RTP );
			
			m_clsCallBack.EventCallStart( strCallId, clsRtp );
		}
		else if( strCommand.equals( EVENT_CALL_END ) )
		{
			String strCallId = b.getString( ARG_CALL_ID );
			int iSipStatus = b.getInt( ARG_SIP_STATUS );
			
			m_clsCallBack.EventCallEnd( strCallId, iSipStatus );
		}
		else if( strCommand.equals( EVENT_REINVITE ) )
		{
			String strCallId = b.getString( ARG_CALL_ID );
			SipCallRtp clsRemoteRtp = (SipCallRtp)b.getSerializable( ARG_REMOTE_RTP );
			SipCallRtp clsLocalRtp = (SipCallRtp)b.getSerializable( ARG_LOCAL_RTP );
			
			m_clsCallBack.EventReInvite( strCallId, clsRemoteRtp, clsLocalRtp );
		}
		else if( strCommand.equals( EVENT_PRACK ) )
		{
			String strCallId = b.getString( ARG_CALL_ID );
			SipCallRtp clsRtp = (SipCallRtp)b.getSerializable( ARG_RTP );
			
			m_clsCallBack.EventPrack( strCallId, clsRtp );
		}
		else if( strCommand.equals( EVENT_TRANSFER ) )
		{
			String strCallId = b.getString( ARG_CALL_ID );
			String strReferToCallId = b.getString( ARG_REFER_TO_CALL_ID );
			boolean bScreenedTransfer = b.getBoolean( ARG_SCREENED_TRANSFER );
			
			m_clsCallBack.EventTransfer( strCallId, strReferToCallId, bScreenedTransfer );
		}
		else if( strCommand.equals( EVENT_BLIND_TRANSFER ) )
		{
			String strCallId = b.getString( ARG_CALL_ID );
			String strReferToId = b.getString( ARG_REFER_TO_ID );
			
			m_clsCallBack.EventBlindTransfer( strCallId, strReferToId );
		}
		else if( strCommand.equals( EVENT_MESSAGE ) )
		{
			String strFrom = b.getString( ARG_FROM );
			String strTo = b.getString( ARG_TO );
			String strSms = b.getString( ARG_SMS );
			
			m_clsCallBack.EventMessage( strFrom, strTo, strSms );
		}
	}

	@Override
	public void EventRegister( SipServerInfo clsInfo, int iStatus )
	{
		Bundle b = new Bundle();
  	
  	Message m = obtainMessage( );
  	b.putString( COMMAND, EVENT_REGISTER );
  	b.putSerializable( ARG_INFO, clsInfo );
  	b.putInt( ARG_STATUS, iStatus );
  	m.setData( b );
  	sendMessage( m );
	}

	@Override
	public void EventIncomingCall( String strCallId, String strFrom, String strTo, SipCallRtp clsRtp )
	{
		Bundle b = new Bundle();
  	
  	Message m = obtainMessage( );
  	b.putString( COMMAND, EVENT_INCOMING_CALL );
  	b.putString( ARG_CALL_ID, strCallId );
  	b.putString( ARG_FROM, strFrom );
  	b.putString( ARG_TO, strTo );
  	b.putSerializable( ARG_RTP, clsRtp );
  	m.setData( b );
  	sendMessage( m );
	}

	@Override
	public void EventCallRing( String strCallId, int iSipStatus, SipCallRtp clsRtp )
	{
		Bundle b = new Bundle();
  	
  	Message m = obtainMessage( );
  	b.putString( COMMAND, EVENT_CALL_RING );
  	b.putString( ARG_CALL_ID, strCallId );
  	b.putInt( ARG_SIP_STATUS, iSipStatus );
  	b.putSerializable( ARG_RTP, clsRtp );
  	m.setData( b );
  	sendMessage( m );
	}

	@Override
	public void EventCallStart( String strCallId, SipCallRtp clsRtp )
	{
		Bundle b = new Bundle();
  	
  	Message m = obtainMessage( );
  	b.putString( COMMAND, EVENT_CALL_START );
  	b.putString( ARG_CALL_ID, strCallId );
  	b.putSerializable( ARG_RTP, clsRtp );
  	m.setData( b );
  	sendMessage( m );
	}

	@Override
	public void EventCallEnd( String strCallId, int iSipStatus )
	{
		Bundle b = new Bundle();
  	
  	Message m = obtainMessage( );
  	b.putString( COMMAND, EVENT_CALL_END );
  	b.putString( ARG_CALL_ID, strCallId );
  	b.putInt( ARG_SIP_STATUS, iSipStatus );
  	m.setData( b );
  	sendMessage( m );
	}

	@Override
	public void EventReInvite( String strCallId, SipCallRtp clsRemoteRtp, SipCallRtp clsLocalRtp )
	{
		Bundle b = new Bundle();
  	
  	Message m = obtainMessage( );
  	b.putString( COMMAND, EVENT_REINVITE );
  	b.putString( ARG_CALL_ID, strCallId );
  	b.putSerializable( ARG_REMOTE_RTP, clsRemoteRtp );
  	b.putSerializable( ARG_LOCAL_RTP, clsLocalRtp );
  	m.setData( b );
  	sendMessage( m );
	}

	@Override
	public void EventPrack( String strCallId, SipCallRtp clsRtp )
	{
		Bundle b = new Bundle();
  	
  	Message m = obtainMessage( );
  	b.putString( COMMAND, EVENT_PRACK );
  	b.putString( ARG_CALL_ID, strCallId );
  	b.putSerializable( ARG_RTP, clsRtp );
  	m.setData( b );
  	sendMessage( m );
	}
	
	@Override
	public void EventTransfer( String strCallId, String strReferToCallId, boolean bScreenedTransfer )
	{
		Bundle b = new Bundle();
  	
  	Message m = obtainMessage( );
  	b.putString( COMMAND, EVENT_TRANSFER );
  	b.putString( ARG_CALL_ID, strCallId );
  	b.putString( ARG_REFER_TO_CALL_ID, strReferToCallId );
  	b.putBoolean( ARG_SCREENED_TRANSFER, bScreenedTransfer );
  	m.setData( b );
  	sendMessage( m );
	}

	@Override
	public void EventBlindTransfer( String strCallId, String strReferToId )
	{
		Bundle b = new Bundle();
  	
  	Message m = obtainMessage( );
  	b.putString( COMMAND, EVENT_BLIND_TRANSFER );
  	b.putString( ARG_CALL_ID, strCallId );
  	b.putString( ARG_REFER_TO_ID, strReferToId );
  	m.setData( b );
  	sendMessage( m );
	}

	@Override
	public void EventMessage( String strFrom, String strTo, String strSms )
	{
		Bundle b = new Bundle();
  	
  	Message m = obtainMessage( );
  	b.putString( COMMAND, EVENT_MESSAGE );
  	b.putString( ARG_FROM, strFrom );
  	b.putString( ARG_TO, strTo );
  	b.putString( ARG_SMS, strSms );
  	m.setData( b );
  	sendMessage( m );
	}

}

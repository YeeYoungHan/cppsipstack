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

package com.example.testandroidsipstack;

import com.cppsipstack.SipCallRtp;
import com.cppsipstack.SipServerInfo;
import com.cppsipstack.SipStackSetup;
import com.cppsipstack.SipUserAgent;
import com.cppsipstack.SipUserAgentCallBack;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.app.Activity;
import android.content.Intent;

/** SIP 클라이언트
 * @author 이영한 ( http://blog.naver.com/websearch )
 */
public class MainActivity extends Activity implements OnClickListener, SipUserAgentCallBack 
{
	EditText m_txtPhoneNumber;
	Button m_btnStartCall, m_btnStopCall, m_btnAcceptCall;
	Button m_btnStartStack, m_btnStopStack;
	TextView m_txtLog;
	String m_strCallId = "";
	Handler	 m_clsHandler;

	@Override
	protected void onCreate( Bundle savedInstanceState )
	{
		super.onCreate( savedInstanceState );
		setContentView( R.layout.activity_main );
		
		Setup.Get( this );
		
		m_txtPhoneNumber = (EditText)findViewById( R.id.txtPhoneNumber );
		m_txtLog = (TextView)findViewById( R.id.txtLog );
		
		m_btnStartCall = (Button)findViewById( R.id.btnStartCall );
		m_btnStopCall = (Button)findViewById( R.id.btnStopCall );
		m_btnAcceptCall = (Button)findViewById( R.id.btnAcceptCall );
		m_btnStartStack = (Button)findViewById( R.id.btnStartStack );
		m_btnStopStack = (Button)findViewById( R.id.btnStopStack );
		
		m_btnStartCall.setOnClickListener( this );
		m_btnStopCall.setOnClickListener( this );
		m_btnAcceptCall.setOnClickListener( this );
		m_btnStartStack.setOnClickListener( this );
		m_btnStopStack.setOnClickListener( this );
		
		findViewById( R.id.btnSetup ).setOnClickListener( this );
		
		SetButtonInit();
		
		m_clsHandler = new Handler()
    {
    	public void handleMessage( Message m )
    	{
    		Bundle b = m.getData( );
    		
    		String strCommand = (String)b.get( "command" );
    		String strData = (String)b.get( "data" );
    		
    		if( strCommand.equals( "log" ) )
    		{
    			m_txtLog.setText( strData );
    		}
    		else if( strCommand.equals( "incoming_call" ) )
    		{
    			m_btnStartCall.setEnabled( false );
    			m_btnStopCall.setEnabled( true );
    			m_btnAcceptCall.setEnabled( true );
    		}
    		else if( strCommand.equals( "start_call" ) )
    		{
    			m_btnStartCall.setEnabled( false );
    			m_btnStopCall.setEnabled( true );
    			m_btnAcceptCall.setEnabled( false );
    		}
    		else if( strCommand.equals( "stop_call" ) )
    		{
    			SetButtonInit();
    		}
    	}
    };
    
    m_txtPhoneNumber.setText( "1001" );
    
    m_btnStopStack.setEnabled( false );
	}
	
	@Override
	protected void onDestroy()
	{
		SipUserAgent.Stop( );
		
		super.onDestroy( );
	}

	@Override
	public void onClick( View v )
	{
		switch( v.getId( ) )
		{
		case R.id.btnStartCall:
			{
				String strPhoneNumber = m_txtPhoneNumber.getText( ).toString( );
				SipCallRtp clsRtp = new SipCallRtp();
				
				clsRtp.m_strIp = Setup.m_strLocalIp;
				clsRtp.m_iPort = 10000;
				clsRtp.m_iCodec = 0;

				m_strCallId = SipUserAgent.StartCall( "1010", strPhoneNumber, clsRtp );
				Log.d( "test", "callid(" + m_strCallId + ")" );
				
				if( m_strCallId != null )
				{
					m_txtLog.setText( "StartCall" );
					
					m_btnStartCall.setEnabled( false );
    			m_btnStopCall.setEnabled( true );
				}
			}
			break;
		case R.id.btnStopCall:
			{
				int iStatusCode = 200;
				
				if( m_btnAcceptCall.isEnabled( ) )
				{
					iStatusCode = 603;
				}
				
				if( SipUserAgent.StopCall( m_strCallId, iStatusCode ) )
				{
					m_txtLog.setText( "Call end" );
					SetButtonInit();
				}
			}
			break;
		case R.id.btnAcceptCall:
			{
				SipCallRtp clsRtp = new SipCallRtp();
				
				clsRtp.m_strIp = Setup.m_strLocalIp;
				clsRtp.m_iPort = 10000;
				clsRtp.m_iCodec = 0;
				
				if( SipUserAgent.AcceptCall( m_strCallId, clsRtp ) )
				{
					m_txtLog.setText( "Call connected" );
					m_btnAcceptCall.setEnabled( false );
				}
			}
			break;
		case R.id.btnSetup:
			{
				Intent clsIntent = new Intent( this, SetupActivity.class );
				startActivity( clsIntent );
			}
			break;
		case R.id.btnStartStack:
			{
				if( Setup.IsSet( ) == false )
				{
					Alert.ShowError( this, "Please click Setup button and setup SIP Server IP and User ID" );
					return;
				}
				
				SipServerInfo clsInfo = new SipServerInfo();
				
				clsInfo.m_strIp = Setup.m_strSipServerIp;
				clsInfo.m_iPort = Setup.m_iSipServerPort;
				clsInfo.m_strDomain = Setup.m_strSipDomain;
				clsInfo.m_strUserId = Setup.m_strSipUserId;
				clsInfo.m_strPassWord = Setup.m_strSipPassWord;
			
				SipUserAgent.InsertRegisterInfo( clsInfo );
				SipUserAgent.SetCallBack( this );
				
				SipStackSetup clsSetup = new SipStackSetup();

				if( SipUserAgent.Start( clsSetup ) )
				{
					m_btnStartStack.setEnabled( false );
					m_btnStopStack.setEnabled( true );
				}
			}
			break;
		case R.id.btnStopStack:
			{
				SipUserAgent.Stop( );
				
				m_btnStartStack.setEnabled( true );
				m_btnStopStack.setEnabled( false );
			}
			break;
		}
	}

	@Override
	public void EventRegister( SipServerInfo clsInfo, int iStatus )
	{
		Send( "log", "EventRegister(" + iStatus + ")" );
	}

	@Override
	public void EventIncomingCall( String strCallId, String strFrom, String strTo, SipCallRtp clsRtp )
	{
		Send( "log", "EventIncomingCall " + strFrom );
		
		m_strCallId = strCallId;
		
		Send( "incoming_call", "" );
	}

	@Override
	public void EventCallRing( String strCallId, int iSipStatus, SipCallRtp clsRtp )
	{
		Send( "log", "EventCallRing(" + iSipStatus + ")" );
	}

	@Override
	public void EventCallStart( String strCallId, SipCallRtp clsRtp )
	{
		Send( "log", "EventCallStart" );
		Send( "start_call", "" );
	}

	@Override
	public void EventCallEnd( String strCallId, int iSipStatus )
	{
		Send( "log", "EventCallEnd" );
		Send( "stop_call", "" );
	}
	
	void Send( String strCommand, String strData )
  {
  	Bundle b = new Bundle();
  	
  	Message m = m_clsHandler.obtainMessage( );
  	b.putString( "command", strCommand );
  	b.putString( "data", strData );
  	m.setData( b );
  	m_clsHandler.sendMessage( m );
  }
	
	void SetButtonInit( )
	{
		m_btnStartCall.setEnabled( true );
		m_btnStopCall.setEnabled( false );
		m_btnAcceptCall.setEnabled( false );
	}
}

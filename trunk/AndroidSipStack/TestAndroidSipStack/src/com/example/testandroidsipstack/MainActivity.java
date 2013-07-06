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
import android.util.Log;
import android.app.Activity;

public class MainActivity extends Activity implements SipUserAgentCallBack
{
	@Override
	protected void onCreate( Bundle savedInstanceState )
	{
		super.onCreate( savedInstanceState );
		setContentView( R.layout.activity_main );
		
		SipServerInfo clsInfo = new SipServerInfo();
		
		clsInfo.m_strIp = "127.0.0.1";
		clsInfo.m_iPort = 5060;
		clsInfo.m_strDomain = "test.com";
		clsInfo.m_strUserId = "userid";
		clsInfo.m_strPassWord = "password";
		
		SipUserAgent.InsertRegisterInfo( clsInfo );
		
		SipStackSetup clsSetup = new SipStackSetup();
		
		SipUserAgent.Start( clsSetup );
	}

	@Override
	public void EventRegister( SipServerInfo clsInfo, int iStatus )
	{
		Log.d( "debug", "EventRegister(" + iStatus + ")" );
	}

	@Override
	public void EventIncomingCall( String strCallId, String strFrom, String strTo, SipCallRtp clsRtp )
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void EventCallRing( String strCallId, int iSipStatus, SipCallRtp clsRtp )
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void EventCallStart( String strCallId, SipCallRtp clsRtp )
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void EventCallEnd( String strCallId, int iSipStatus )
	{
		// TODO Auto-generated method stub
		
	}
}

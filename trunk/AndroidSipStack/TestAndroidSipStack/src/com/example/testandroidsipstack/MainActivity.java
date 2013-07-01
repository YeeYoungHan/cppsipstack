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

import com.cppsipstack.SipServerInfo;
import com.cppsipstack.SipUserAgent;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;

public class MainActivity extends Activity
{

	@Override
	protected void onCreate( Bundle savedInstanceState )
	{
		super.onCreate( savedInstanceState );
		setContentView( R.layout.activity_main );
		
		SipServerInfo clsInfo = new SipServerInfo();
		
		clsInfo.m_strIp = "127.0.0.1";
		clsInfo.m_iPort = 5060;
		
		SipUserAgent.InsertRegisterInfo( clsInfo );
	}

	@Override
	public boolean onCreateOptionsMenu( Menu menu )
	{
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater( ).inflate( R.menu.main, menu );
		return true;
	}

}

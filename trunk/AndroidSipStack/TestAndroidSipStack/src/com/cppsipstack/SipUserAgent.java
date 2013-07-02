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

import android.util.Log;

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
		
	}
	
	static
	{
  	System.out.println( "load AndroidSipStack" );
		
		try
		{
			System.loadLibrary( "CppSipStack" );
		}
		catch( Throwable e )
		{
			Log.e( "AndroidSipStack", "loadLibrary error - " + e.toString( ) );
		}
  }
}

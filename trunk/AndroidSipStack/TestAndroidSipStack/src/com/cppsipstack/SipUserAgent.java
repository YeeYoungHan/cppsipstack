package com.cppsipstack;

import android.util.Log;

public class SipUserAgent
{
	public static native boolean InsertRegisterInfo( SipServerInfo clsInfo );
	
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

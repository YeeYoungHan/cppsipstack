package com.cppsipstack;

import android.util.Log;

public class SipLog
{
	public static String LOG_TAG = "AndroidSipStack";
	
	public static void Debug( String strText )
	{
		Log.d( LOG_TAG, strText );
	}
	
	public static void Error( String strText )
	{
		Log.e( LOG_TAG, strText );
	}
}

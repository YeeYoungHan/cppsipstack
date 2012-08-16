package com.curio.sip;

import java.util.Random;
import java.util.StringTokenizer;
import android.util.Log;

public class SipManager {	
	public static String TAG = "CURIO";
	
    static {
    	System.loadLibrary("SipUA_so");
    }

	public SipManager(UIInterface interf, Object syncObj)
	{
	}
	
	public void close()
	{
		return;
	}
	
	public int Initialize( String id, 
									String authId, 
									String authPwd, 
									int nRecvPort,
									int nSendPort,
									String svrIp, 
									String domain, 
									int nMediaSPort, 
									int nMediaEPort,
									int nProto,
									String localIp,
									String callId,
									int nExpire,
									String strLogPath,
									int keepCount)
	{
		int nRet = 0;
		return nRet;
	}
	
	public boolean Register()
	{	
		boolean bRet = true;
		return bRet;
	}
	
	public static int GetPortNumber()
	{
		int nRandomNum = 0;
		return nRandomNum;
	}
	
	public boolean Accept(String id)
	{
		boolean bRet = true;
		return bRet;
	}
	
	public boolean Cancel(String id)
	{
		boolean bRet = true;
		return bRet;
	}
	
	public boolean Hold(String id)
	{
		boolean bRet = true;
		return bRet;
	}
	
	public boolean Bye(String id)
	{
		boolean bRet = true;
		return bRet;
	}
	
	public static boolean Reject(String id)
	{
		boolean bRet = true;
		return bRet;
	}
	
	public int Invite(String id)
	{
		int bRet = 0;
		return bRet;
	}
}
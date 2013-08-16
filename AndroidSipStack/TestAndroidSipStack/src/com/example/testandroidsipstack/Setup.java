package com.example.testandroidsipstack;

import java.io.BufferedReader;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

import android.content.Context;

public class Setup
{
	private static final String m_strEqual = "=";
	private static final String m_strNewLine = "\n";
	private static final String m_strFileName = "Setup.ini";
	
	// 설정 항목의 키 값
	private static final String m_strSipServerIpKey = "SipServerIp";
	private static final String m_strSipServerPortKey = "SipServerPort";
	private static final String m_strSipDomainKey = "SipDomain";
	private static final String m_strSipUserIdKey = "SipUserId";
	private static final String m_strSipPassWordKey = "SipPassWord";
	
	// 설정 항목 저장 변수
	public static String m_strSipServerIp = "";
	public static int m_iSipServerPort = 5060;
	public static String m_strSipDomain = "";
	public static String m_strSipUserId = "";
	public static String m_strSipPassWord = "";
	
	// 전역변수
	public static String m_strLocalIp = "";
	
	public static boolean IsSet()
	{
		if( m_strSipServerIp.length( ) == 0 ) return false;
		if( m_strSipUserId.length( ) == 0 ) return false;
		
		return true;
	}
	
	/** 설정 파일에서 설정 항목을 읽어서 내부 변수에 저장한다.
	 * @param clsContext 안드로이드 Context
	 * @return 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
	 */
	public static boolean Get( Context clsContext )
	{
		boolean bRes = false;
		String strLine;
		
		try
    {
      BufferedReader clsInput = new BufferedReader( new InputStreamReader( clsContext.openFileInput( m_strFileName ) ) );
      
      while( ( strLine = clsInput.readLine() ) != null )
      {
        String [] arrData = strLine.split( m_strEqual );
        
        if( arrData.length != 2 ) continue;
        
        if( arrData[0].equals( m_strSipServerIpKey ) )
        {
        	m_strSipServerIp = arrData[1];
        }
        else if( arrData[0].equals( m_strSipServerPortKey ) )
        {
        	m_iSipServerPort = GetInt( arrData[1], m_iSipServerPort );
        }
        else if( arrData[0].equals( m_strSipDomainKey ) )
        {
        	m_strSipDomain = arrData[1];
        }
        else if( arrData[0].equals( m_strSipUserIdKey ) )
        {
        	m_strSipUserId = arrData[1];
        }
        else if( arrData[0].equals( m_strSipPassWordKey ) )
        {
        	m_strSipPassWord = arrData[1];
        }
      }
      
      clsInput.close( );
      
      bRes = true;
    }
    catch( Exception e )
    {
    	bRes = false;
    }
		
		m_strLocalIp = getLocalIpAddress();
		
		return bRes;
	}
	

	/** 설정 항목을 설정 파일에 저장한다.
	 * @param clsContext 안드로이드 Context
	 * @return 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
	 */
	public static boolean Put( Context clsContext )
	{
		boolean bRes = false;
		
		if( m_strSipDomain.length( ) == 0 ) m_strSipDomain = m_strSipServerIp;
		
		try
		{
			FileOutputStream clsOutput = clsContext.openFileOutput( m_strFileName, Context.MODE_PRIVATE );
			
			Write( clsOutput, m_strSipServerIpKey, m_strSipServerIp );
			Write( clsOutput, m_strSipServerPortKey, m_iSipServerPort );
			Write( clsOutput, m_strSipDomainKey, m_strSipDomain );
			Write( clsOutput, m_strSipUserIdKey, m_strSipUserId );
			Write( clsOutput, m_strSipPassWordKey, m_strSipPassWord );
			
			clsOutput.close( );
			bRes = true;
		}
		catch( Exception e )
    {
    }
		
		return bRes;
	}
	
	/** 하나의 설정 항목을 설정 파일에 저장한다.
	 * @param clsOutput			설정 파일 저장용 스트림
	 * @param strName				설정 키
	 * @param strValue			설정 값
	 * @throws IOException 입력 오류
	 */
	private static void Write( FileOutputStream clsOutput, String strName, String strValue ) throws IOException
	{
		clsOutput.write( strName.getBytes( ) );
		clsOutput.write( m_strEqual.getBytes( ) );
		clsOutput.write( strValue.getBytes( ) );
		clsOutput.write( m_strNewLine.getBytes( ) );	
	}
	
	private static void Write( FileOutputStream clsOutput, String strName, int iValue ) throws IOException
	{
		Write( clsOutput, strName, "" + iValue );	
	}
	
	private static int GetInt( String strValue, int iDefault )
	{
		if( strValue == null ) return iDefault;

		int iNum;
		
		try
		{
			iNum = Integer.parseInt( strValue );
		}
		catch( Exception e )
		{
			iNum = iDefault;
		}
		
		return iNum;
	}
	
	/** 로컬 IP 주소를 검색하여서 리턴한다.
	 * @return 로컬 IP 주소를 검색하여서 리턴한다.
	 */
	public static String getLocalIpAddress()
	{
		try
		{
			for( Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements();)
			{
				NetworkInterface intf = en.nextElement();
				for( Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements();)
				{
					InetAddress inetAddress = enumIpAddr.nextElement();
					if( !inetAddress.isLoopbackAddress() )
					{
						return inetAddress.getHostAddress().toString();
					}
				}
			}
		}
		catch( SocketException ex )
		{

		}

		return null;
  }
}

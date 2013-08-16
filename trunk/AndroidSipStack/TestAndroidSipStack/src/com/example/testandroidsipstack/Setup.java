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
	
	// ���� �׸��� Ű ��
	private static final String m_strSipServerIpKey = "SipServerIp";
	private static final String m_strSipServerPortKey = "SipServerPort";
	private static final String m_strSipDomainKey = "SipDomain";
	private static final String m_strSipUserIdKey = "SipUserId";
	private static final String m_strSipPassWordKey = "SipPassWord";
	
	// ���� �׸� ���� ����
	public static String m_strSipServerIp = "";
	public static int m_iSipServerPort = 5060;
	public static String m_strSipDomain = "";
	public static String m_strSipUserId = "";
	public static String m_strSipPassWord = "";
	
	// ��������
	public static String m_strLocalIp = "";
	
	public static boolean IsSet()
	{
		if( m_strSipServerIp.length( ) == 0 ) return false;
		if( m_strSipUserId.length( ) == 0 ) return false;
		
		return true;
	}
	
	/** ���� ���Ͽ��� ���� �׸��� �о ���� ������ �����Ѵ�.
	 * @param clsContext �ȵ���̵� Context
	 * @return �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
	

	/** ���� �׸��� ���� ���Ͽ� �����Ѵ�.
	 * @param clsContext �ȵ���̵� Context
	 * @return �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
	
	/** �ϳ��� ���� �׸��� ���� ���Ͽ� �����Ѵ�.
	 * @param clsOutput			���� ���� ����� ��Ʈ��
	 * @param strName				���� Ű
	 * @param strValue			���� ��
	 * @throws IOException �Է� ����
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
	
	/** ���� IP �ּҸ� �˻��Ͽ��� �����Ѵ�.
	 * @return ���� IP �ּҸ� �˻��Ͽ��� �����Ѵ�.
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

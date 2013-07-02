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

bool GetSipServerInfo( JNIEnv * env, jobject clsSipServerInfo, CSipServerInfo & clsOutput )
{
	jclass clsJava = env->GetObjectClass( clsSipServerInfo );
	if( clsJava == NULL )
	{
		AndroidErrorLog( "clsSipServerInfo is not found" );
		return false;
	}

	if( GetString( env, clsSipServerInfo, clsJava, "m_strIp", clsOutput.m_strIp ) == false ) return false;
	if( GetInt( env, clsSipServerInfo, clsJava, "m_iPort", clsOutput.m_iPort ) == false ) return false;
	if( GetString( env, clsSipServerInfo, clsJava, "m_strDomain", clsOutput.m_strDomain ) == false ) return false;
	if( GetString( env, clsSipServerInfo, clsJava, "m_strUserId", clsOutput.m_strUserId ) == false ) return false;
	if( GetString( env, clsSipServerInfo, clsJava, "m_strPassWord", clsOutput.m_strPassWord ) == false ) return false;

	if( gbAndroidDebug )
	{
		AndroidDebugLog( "ip(%s) port(%d) domain(%s) userid(%s) password(%s)"
			, clsOutput.m_strIp.c_str(), clsOutput.m_iPort, clsOutput.m_strDomain.c_str()
			, clsOutput.m_strUserId.c_str(), clsOutput.m_strPassWord.c_str() );
	}

	return true;
}

bool GetSipStackSetup( JNIEnv * env, jobject clsSipStackSetup, CSipStackSetup & clsOutput )
{
	jclass clsJava = env->GetObjectClass( clsSipStackSetup );
	if( clsJava == NULL )
	{
		AndroidErrorLog( "clsSipStackSetup is not found" );
		return false;
	}

	if( GetString( env, clsSipStackSetup, clsJava, "m_strLocalIp", clsOutput.m_strLocalIp ) == false ) return false;
	if( GetInt( env, clsSipStackSetup, clsJava, "m_iLocalUdpPort", clsOutput.m_iLocalUdpPort ) == false ) return false;
	if( GetInt( env, clsSipStackSetup, clsJava, "m_iUdpThreadCount", clsOutput.m_iUdpThreadCount ) == false ) return false;
	if( GetInt( env, clsSipStackSetup, clsJava, "m_iLocalTcpPort", clsOutput.m_iLocalTcpPort ) == false ) return false;
	if( GetInt( env, clsSipStackSetup, clsJava, "m_iLocalTlsPort", clsOutput.m_iLocalTlsPort ) == false ) return false;
	if( GetInt( env, clsSipStackSetup, clsJava, "m_iTcpThreadCount", clsOutput.m_iTcpThreadCount ) == false ) return false;
	if( GetInt( env, clsSipStackSetup, clsJava, "m_iTcpMaxSocketPerThread", clsOutput.m_iTcpMaxSocketPerThread ) == false ) return false;
	if( GetInt( env, clsSipStackSetup, clsJava, "m_iTcpRecvTimeout", clsOutput.m_iTcpRecvTimeout ) == false ) return false;
	if( GetInt( env, clsSipStackSetup, clsJava, "m_iTlsAcceptTimeout", clsOutput.m_iTlsAcceptTimeout ) == false ) return false;
	if( GetString( env, clsSipStackSetup, clsJava, "m_strCertFile", clsOutput.m_strCertFile ) == false ) return false;
	if( GetString( env, clsSipStackSetup, clsJava, "m_strUserAgent", clsOutput.m_strUserAgent ) == false ) return false;
	if( GetBool( env, clsSipStackSetup, clsJava, "m_bUseSipCompactForm", clsOutput.m_bUseSipCompactForm ) == false ) return false;
	
	return true;
}

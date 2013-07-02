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
	
	AndroidDebugLog( "ip(%s) port(%d) domain(%s) userid(%s) password(%s)"
		, clsOutput.m_strIp.c_str(), clsOutput.m_iPort, clsOutput.m_strDomain.c_str()
		, clsOutput.m_strUserId.c_str(), clsOutput.m_strPassWord.c_str() );

	return true;
}

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

#include "AndroidGet.h"
#include "AndroidLog.h"

bool GetString( JNIEnv * env, jobject clsSipServerInfo, jclass clsJava, const char * pszName, std::string & strOutput )
{
	jfieldID fid = env->GetFieldID( clsJava, pszName, "Ljava/lang/String;" );
	if( fid == NULL )
	{
		AndroidErrorLog( "fid(%s) is not found", pszName );
		return false;
	}
	
	jstring strData = (jstring)env->GetObjectField( clsSipServerInfo, fid );
	const char * pszData = env->GetStringUTFChars( strData, NULL );
	
	strOutput = pszData;
	
	env->ReleaseStringUTFChars( strData, pszData );

	return true;
}

bool GetInt( JNIEnv * env, jobject clsSipServerInfo, jclass clsJava, const char * pszName, int & iOutput )
{
	jfieldID fid = env->GetFieldID( clsJava, pszName, "I" );
	if( fid == NULL )
	{
		AndroidErrorLog( "fid(%s) is not found", pszName );
		return false;
	}

	iOutput = env->GetIntField( clsSipServerInfo, fid );

	return true;
}

bool GetBool( JNIEnv * env, jobject clsSipServerInfo, jclass clsJava, const char * pszName, bool & bOutput )
{
	jfieldID fid = env->GetFieldID( clsJava, pszName, "Z" );
	if( fid == NULL )
	{
		AndroidErrorLog( "fid(%s) is not found", pszName );
		return false;
	}

	jboolean b = env->GetBooleanField( clsSipServerInfo, fid );

	if( b == JNI_TRUE )
	{
		bOutput = true;
	}
	else
	{
		bOutput = false;
	}

	return true;
}

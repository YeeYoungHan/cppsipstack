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

/**
 * @brief 객체의 멤버 문자열 변수에 저장된 값을 가져온다.
 * @param env				JNIEnv
 * @param jObject		java 객체
 * @param jClass		java 클래스
 * @param pszName		멤버 문자열 변수 이름
 * @param strOutput 멤버 문자열 변수의 값을 저장할 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool GetString( JNIEnv * env, jobject jObject, jclass jClass, const char * pszName, std::string & strOutput )
{
	strOutput.clear();

	jfieldID fid = env->GetFieldID( jClass, pszName, "Ljava/lang/String;" );
	if( fid == NULL )
	{
		AndroidErrorLog( "fid(%s) is not found", pszName );
		return false;
	}
	
	jstring strData = (jstring)env->GetObjectField( jObject, fid );
	if( strData == NULL )
	{
		AndroidErrorLog( "fid(%s)'s string is not found", pszName );
		return true;
	}

	const char * pszData = env->GetStringUTFChars( strData, NULL );
	if( pszData == NULL ) 
	{
		AndroidErrorLog( "fid(%s)'s string is null", pszName );
		return false;
	}

	strOutput = pszData;
	
	env->ReleaseStringUTFChars( strData, pszData );

	return true;
}

/**
 * @brief 객체의 멤버 정수 변수에 저장된 값을 가져온다.
 * @param env				JNIEnv
 * @param jObject		java 객체
 * @param jClass		java 클래스
 * @param pszName		멤버 정수 변수 이름
 * @param iOutput		멤버 정수 변수의 값을 저장할 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool GetInt( JNIEnv * env, jobject jObject, jclass jClass, const char * pszName, int & iOutput )
{
	jfieldID fid = env->GetFieldID( jClass, pszName, "I" );
	if( fid == NULL )
	{
		AndroidErrorLog( "fid(%s) is not found", pszName );
		return false;
	}

	iOutput = env->GetIntField( jObject, fid );

	return true;
}

/**
 * @brief 객체의 멤버 boolean 변수에 저장된 값을 가져온다.
 * @param env				JNIEnv
 * @param jObject		java 객체
 * @param jClass		java 클래스
 * @param pszName		멤버 boolean 변수 이름
 * @param bOutput		멤버 boolean 변수의 값을 저장할 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool GetBool( JNIEnv * env, jobject jObject, jclass jClass, const char * pszName, bool & bOutput )
{
	jfieldID fid = env->GetFieldID( jClass, pszName, "Z" );
	if( fid == NULL )
	{
		AndroidErrorLog( "fid(%s) is not found", pszName );
		return false;
	}

	jboolean b = env->GetBooleanField( jObject, fid );

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

/**
 * @brief java 문자열 변수에 저장된 값을 가져온다.
 * @param env				JNIEnv
 * @param js				java 문자열 변수
 * @param strOutput java 문자열 변수의 값을 저장할 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool GetString( JNIEnv * env, jstring js, std::string & strOutput )
{
	strOutput.clear();

	const char * pszData = env->GetStringUTFChars( js, NULL );
	if( pszData == NULL ) return false;
	
	strOutput = pszData;
	
	env->ReleaseStringUTFChars( js, pszData );

	return true;
}

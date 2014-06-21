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
 * @ingroup AndroidSipStack
 * @brief ��ü�� ��� ���ڿ� ������ ����� ���� �����´�.
 * @param env				JNIEnv
 * @param jObject		java ��ü
 * @param jClass		java Ŭ����
 * @param pszName		��� ���ڿ� ���� �̸�
 * @param strOutput ��� ���ڿ� ������ ���� ������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @ingroup AndroidSipStack
 * @brief ��ü�� ��� ���� ������ ����� ���� �����´�.
 * @param env				JNIEnv
 * @param jObject		java ��ü
 * @param jClass		java Ŭ����
 * @param pszName		��� ���� ���� �̸�
 * @param iOutput		��� ���� ������ ���� ������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @ingroup AndroidSipStack
 * @brief ��ü�� ��� boolean ������ ����� ���� �����´�.
 * @param env				JNIEnv
 * @param jObject		java ��ü
 * @param jClass		java Ŭ����
 * @param pszName		��� boolean ���� �̸�
 * @param bOutput		��� boolean ������ ���� ������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @ingroup AndroidSipStack
 * @brief java ���ڿ� ������ ����� ���� �����´�.
 * @param env				JNIEnv
 * @param js				java ���ڿ� ����
 * @param strOutput java ���ڿ� ������ ���� ������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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

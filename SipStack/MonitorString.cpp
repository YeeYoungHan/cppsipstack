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

#include "SipStackDefine.h"
#include "MonitorString.h"
#include "TimeString.h"
#include "MemoryDebug.h"

CMonitorString::CMonitorString()
{
}

CMonitorString::~CMonitorString()
{
}

/**
 * @ingroup SipStack
 * @brief 컬럼 문자열을 저장한다.
 * @param pszValue 문자열
 */
void CMonitorString::AddCol( const char * pszValue )
{
	m_strBuf.append( pszValue );
	m_strBuf.append( MR_COL_SEP );
}

/**
 * @ingroup SipStack
 * @brief 컬럼 문자열을 저장한다.
 * @param strValue 문자열
 */
void CMonitorString::AddCol( const std::string & strValue )
{
	m_strBuf.append( strValue );
	m_strBuf.append( MR_COL_SEP );
}

/**
 * @ingroup SipStack
 * @brief 컬럼 문자열에 IP 주소와 포트 번호를 저장한다.
 * @param strIp IP 주소
 * @param iPort 포트 번호
 */
void CMonitorString::AddCol( const std::string & strIp, int iPort )
{
	m_strBuf.append( strIp );
	m_strBuf.append( ":" );
	AddCol( iPort );
}

/**
 * @ingroup SipStack
 * @brief 컬럼 문자열에 숫자를 저장한다.
 * @param iValue 숫자
 */
void CMonitorString::AddCol( int iValue )
{
	char	szValue[21];

	snprintf( szValue, sizeof(szValue), "%d", iValue );
	AddCol( szValue );
}

/**
 * @ingroup SipStack
 * @brief 컬럼 문자열에 시간 문자열을 저장한다.
 * @param iTime 시간
 */
void CMonitorString::AddCol( time_t iTime )
{
	char	szTime[21];

	GetDateTimeString( iTime, szTime, sizeof(szTime) );
	AddCol( szTime );
}

/**
 * @ingroup SipStack
 * @brief 줄의 마지막 문자열을 저장한다.
 * @param pszValue 문자열
 */
void CMonitorString::AddRow( const char * pszValue )
{
	m_strBuf.append( pszValue );
	m_strBuf.append( MR_ROW_SEP );
}

/**
 * @ingroup SipStack
 * @brief 줄의 마지막 문자열을 저장한다.
 * @param strValue 문자열
 */
void CMonitorString::AddRow( const std::string & strValue )
{
	m_strBuf.append( strValue );
	m_strBuf.append( MR_ROW_SEP );
}

/**
 * @ingroup SipStack
 * @brief 줄의 마지막 문자열에 IP 주소와 포트 번호를 저장한다.
 * @param strIp IP 주소
 * @param iPort 포트 번호
 */
void CMonitorString::AddRow( const std::string & strIp, int iPort )
{
	m_strBuf.append( strIp );
	m_strBuf.append( ":" );
	AddRow( iPort );
}

/**
 * @ingroup SipStack
 * @brief 줄의 마지막 문자열에 숫자를 저장한다.
 * @param iValue 숫자
 */
void CMonitorString::AddRow( int iValue )
{
	char	szValue[21];

	snprintf( szValue, sizeof(szValue), "%d", iValue );
	AddRow( szValue );
}

/**
 * @ingroup SipStack
 * @brief 줄의 마지막 문자열에 시간 문자열을 저장한다.
 * @param iTime 시간
 */
void CMonitorString::AddRow( time_t iTime )
{
	char	szTime[21];

	GetDateTimeString( iTime, szTime, sizeof(szTime) );
	AddRow( szTime );
}

/**
 * @ingroup SipStack
 * @brief 저장된 문자열을 초기화시킨다.
 */
void CMonitorString::Clear( )
{
	m_strBuf.clear();
}

/**
 * @ingroup SipStack
 * @brief 저장된 문자열을 리턴한다.
 * @returns 저장된 문자열을 리턴한다.
 */
const char * CMonitorString::GetString()
{
	return m_strBuf.c_str();
}

/**
 * @ingroup SipStack
 * @brief 저장된 문자열의 길이를 리턴한다.
 * @returns 저장된 문자열의 길이를 리턴한다.
 */
int CMonitorString::GetLength()
{
	return (int)m_strBuf.length();
}

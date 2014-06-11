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
 * @brief �÷� ���ڿ��� �����Ѵ�.
 * @param pszValue ���ڿ�
 */
void CMonitorString::AddCol( const char * pszValue )
{
	m_strBuf.append( pszValue );
	m_strBuf.append( MR_COL_SEP );
}

/**
 * @ingroup SipStack
 * @brief �÷� ���ڿ��� �����Ѵ�.
 * @param strValue ���ڿ�
 */
void CMonitorString::AddCol( const std::string & strValue )
{
	m_strBuf.append( strValue );
	m_strBuf.append( MR_COL_SEP );
}

/**
 * @ingroup SipStack
 * @brief �÷� ���ڿ��� IP �ּҿ� ��Ʈ ��ȣ�� �����Ѵ�.
 * @param strIp IP �ּ�
 * @param iPort ��Ʈ ��ȣ
 */
void CMonitorString::AddCol( const std::string & strIp, int iPort )
{
	m_strBuf.append( strIp );
	m_strBuf.append( ":" );
	AddCol( iPort );
}

/**
 * @ingroup SipStack
 * @brief �÷� ���ڿ��� ���ڸ� �����Ѵ�.
 * @param iValue ����
 */
void CMonitorString::AddCol( int iValue )
{
	char	szValue[21];

	snprintf( szValue, sizeof(szValue), "%d", iValue );
	AddCol( szValue );
}

/**
 * @ingroup SipStack
 * @brief �÷� ���ڿ��� �ð� ���ڿ��� �����Ѵ�.
 * @param iTime �ð�
 */
void CMonitorString::AddCol( time_t iTime )
{
	char	szTime[21];

	GetDateTimeString( iTime, szTime, sizeof(szTime) );
	AddCol( szTime );
}

/**
 * @ingroup SipStack
 * @brief ���� ������ ���ڿ��� �����Ѵ�.
 * @param pszValue ���ڿ�
 */
void CMonitorString::AddRow( const char * pszValue )
{
	m_strBuf.append( pszValue );
	m_strBuf.append( MR_ROW_SEP );
}

/**
 * @ingroup SipStack
 * @brief ���� ������ ���ڿ��� �����Ѵ�.
 * @param strValue ���ڿ�
 */
void CMonitorString::AddRow( const std::string & strValue )
{
	m_strBuf.append( strValue );
	m_strBuf.append( MR_ROW_SEP );
}

/**
 * @ingroup SipStack
 * @brief ���� ������ ���ڿ��� IP �ּҿ� ��Ʈ ��ȣ�� �����Ѵ�.
 * @param strIp IP �ּ�
 * @param iPort ��Ʈ ��ȣ
 */
void CMonitorString::AddRow( const std::string & strIp, int iPort )
{
	m_strBuf.append( strIp );
	m_strBuf.append( ":" );
	AddRow( iPort );
}

/**
 * @ingroup SipStack
 * @brief ���� ������ ���ڿ��� ���ڸ� �����Ѵ�.
 * @param iValue ����
 */
void CMonitorString::AddRow( int iValue )
{
	char	szValue[21];

	snprintf( szValue, sizeof(szValue), "%d", iValue );
	AddRow( szValue );
}

/**
 * @ingroup SipStack
 * @brief ���� ������ ���ڿ��� �ð� ���ڿ��� �����Ѵ�.
 * @param iTime �ð�
 */
void CMonitorString::AddRow( time_t iTime )
{
	char	szTime[21];

	GetDateTimeString( iTime, szTime, sizeof(szTime) );
	AddRow( szTime );
}

/**
 * @ingroup SipStack
 * @brief ����� ���ڿ��� �ʱ�ȭ��Ų��.
 */
void CMonitorString::Clear( )
{
	m_strBuf.clear();
}

/**
 * @ingroup SipStack
 * @brief ����� ���ڿ��� �����Ѵ�.
 * @returns ����� ���ڿ��� �����Ѵ�.
 */
const char * CMonitorString::GetString()
{
	return m_strBuf.c_str();
}

/**
 * @ingroup SipStack
 * @brief ����� ���ڿ��� ���̸� �����Ѵ�.
 * @returns ����� ���ڿ��� ���̸� �����Ѵ�.
 */
int CMonitorString::GetLength()
{
	return (int)m_strBuf.length();
}

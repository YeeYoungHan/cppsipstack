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

#include "LogAnalysisSetup.h"
#include "XmlElement.h"

CLogAnalysisSetup gclsSetup;

/**
 * @ingroup KSipServer
 * @brief 생성자
 */
CLogAnalysisSetup::CLogAnalysisSetup() : m_iDbPort(3306)
{
}

/**
 * @ingroup KSipServer
 * @brief 소멸자
 */
CLogAnalysisSetup::~CLogAnalysisSetup()
{
}

/**
 * @ingroup KSipServer
 * @brief 설정 파일을 읽어서 멤버 변수에 저장한다.
 * @param pszFileName 설정 파일 full path
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CLogAnalysisSetup::Read( const char * pszFileName )
{
	CXmlElement clsXml, * pclsElement;

	if( clsXml.ParseFile( pszFileName ) == false ) return false;

	// 로그
	pclsElement = clsXml.SelectElement( "Log" );
	if( pclsElement == NULL ) return false;

	pclsElement->SelectElementData( "Folder", m_strLogFolder );

	// DB
	pclsElement = clsXml.SelectElement( "Db" );
	if( pclsElement )
	{
		int iTemp;

		pclsElement->SelectElementData( "Host", m_strDbHost );
		pclsElement->SelectElementData( "UserId", m_strDbUserId );
		pclsElement->SelectElementData( "PassWord", m_strDbPassWord );
		pclsElement->SelectElementData( "DataBase", m_strDbName );

		if( pclsElement->SelectElementData( "Port", iTemp ) )
		{
			m_iDbPort = iTemp;
		}
	}

	if( m_strDbHost.empty() )
	{
		printf( "[ERROR] Db.Host data is not found\n" );
		return false;
	}

	if( m_strDbUserId.empty() )
	{
		printf( "[ERROR] Db.UserId data is not found\n" );
		return false;
	}

	if( m_strDbName.empty() )
	{
		printf( "[ERROR] Db.DataBase data is not found\n" );
		return false;
	}

	return true;
}

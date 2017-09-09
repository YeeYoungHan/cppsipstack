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
#include <stdio.h>

CLogAnalysisSetup gclsSetup;

/**
 * @ingroup KSipServerLogAnalysis
 * @brief 생성자
 */
CLogAnalysisSetup::CLogAnalysisSetup()
{
}

/**
 * @ingroup KSipServerLogAnalysis
 * @brief 소멸자
 */
CLogAnalysisSetup::~CLogAnalysisSetup()
{
}

/**
 * @ingroup KSipServerLogAnalysis
 * @brief 설정 파일을 읽어서 멤버 변수에 저장한다.
 * @param pszFileName 설정 파일 full path
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CLogAnalysisSetup::Read( const char * pszFileName )
{
	CXmlElement clsXml, * pclsElement;

	if( clsXml.ParseFile( pszFileName ) == false ) return false;

	// 출력 폴더
	pclsElement = clsXml.SelectElement( "Result" );
	if( pclsElement == NULL ) return false;

	if( pclsElement->SelectElementData( "Folder", m_strResultFolder ) == false )
	{
		return false;
	}

	// 로그
	pclsElement = clsXml.SelectElement( "Log" );
	if( pclsElement == NULL ) return false;

	if( pclsElement->SelectElementData( "Folder", m_strLogFolder ) == false )
	{
		return false;
	}

	return true;
}

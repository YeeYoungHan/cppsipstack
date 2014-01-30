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
 * @brief ������
 */
CLogAnalysisSetup::CLogAnalysisSetup()
{
}

/**
 * @ingroup KSipServerLogAnalysis
 * @brief �Ҹ���
 */
CLogAnalysisSetup::~CLogAnalysisSetup()
{
}

/**
 * @ingroup KSipServerLogAnalysis
 * @brief ���� ������ �о ��� ������ �����Ѵ�.
 * @param pszFileName ���� ���� full path
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CLogAnalysisSetup::Read( const char * pszFileName )
{
	CXmlElement clsXml, * pclsElement;

	if( clsXml.ParseFile( pszFileName ) == false ) return false;

	// ��� ����
	pclsElement = clsXml.SelectElement( "Result" );
	if( pclsElement == NULL ) return false;

	if( pclsElement->SelectElementData( "Folder", m_strResultFolder ) == false )
	{
		return false;
	}

	// �α�
	pclsElement = clsXml.SelectElement( "Log" );
	if( pclsElement == NULL ) return false;

	if( pclsElement->SelectElementData( "Folder", m_strLogFolder ) == false )
	{
		return false;
	}

	return true;
}

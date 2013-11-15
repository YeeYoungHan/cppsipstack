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

#include "KSipServer.h"
#include "SipParserDefine.h"
#include "XmlUser.h"
#include "XmlElement.h"
#include "SipServerSetup.h"
#include "Directory.h"
#include "DbMySQL.h"

CXmlUser::CXmlUser() : m_bDnd(false)
{
}

CXmlUser::~CXmlUser()
{
}

/**
 * @ingroup KSipServer
 * @brief XML ������ �о SIP ����� ������ �����´�.
 * @param pszFileName XML ���� �̸�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlUser::Parse( const char * pszFileName )
{
	CXmlElement clsXml;

	Clear();

	if( clsXml.ParseFile( pszFileName ) == false ) return false;

	clsXml.SelectElementData( "Id", m_strId );
	clsXml.SelectElementData( "PassWord", m_strPassWord );

	std::string	strTemp;

	if( clsXml.SelectElementData( "DND", strTemp ) )
	{
		if( !strcasecmp( strTemp.c_str(), "true" ) )
		{
			m_bDnd = true;
		}
	}

	clsXml.SelectElementData( "CallForward", m_strCallForward );
	clsXml.SelectElementData( "GroupId", m_strGroupId );

	if( m_strId.empty() ) return false;

	return true;
}

/**
 * @ingroup KSipServer
 * @brief ���� ������ �ʱ�ȭ��Ų��.
 */
void CXmlUser::Clear()
{
	m_strId.clear();
	m_strPassWord.clear();
	m_bDnd = false;
	m_strCallForward.clear();
}

bool CXmlUser::IsDnd()
{
	return m_bDnd;
}

bool CXmlUser::IsCallForward()
{
	if( m_strCallForward.empty() ) return false;

	return true;
}

#ifdef USE_MYSQL
/**
 * @ingroup KSipServer
 * @brief 
 * @param pclsData 
 * @param sttRow 
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
static bool SipUserFetchRow( void * pclsData, MYSQL_ROW & sttRow )
{
	CXmlUser * pclsUser = (CXmlUser *)pclsData;

	if( sttRow[0] ) pclsUser->m_strPassWord = sttRow[0];
	if( sttRow[1] && sttRow[1][0] == 'Y' ) pclsUser->m_bDnd = true;
	if( sttRow[2] ) pclsUser->m_strCallForward = sttRow[2];

	return true;
}
#endif

/**
 * @ingroup KSipServer
 * @brief SIP ����� ������ XML ���� �Ǵ� MySQL ���� �о �����Ѵ�.
 * @param pszUserId SIP ����� ���̵�
 * @param clsUser		SIP ����� ���� ���� ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool SelectUser( const char * pszUserId, CXmlUser & clsUser )
{	
	if( gclsSetup.m_eType == E_DT_XML )
	{
		std::string	strFileName = gclsSetup.m_strUserXmlFolder;

		CDirectory::AppendName( strFileName, pszUserId );
		strFileName.append( ".xml" );

		return clsUser.Parse( strFileName.c_str() );
	}
#ifdef USE_MYSQL
	else if( gclsSetup.m_eType == E_DT_MYSQL )
	{
		char szSQL[255];

		clsUser.Clear();

		snprintf( szSQL, sizeof(szSQL), "SELECT PassWord, DND, CallForward FROM SipUser WHERE Id = '%s'", pszUserId );
		if( gclsReadDB.Query( szSQL, &clsUser, SipUserFetchRow ) )
		{
			clsUser.m_strId = pszUserId;
			return true;
		}
	}
#endif
	
	return false;
}

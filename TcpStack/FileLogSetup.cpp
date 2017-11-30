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

#include "SipPlatformDefine.h"
#include "FileLogSetup.h"
#include "Log.h"
#include "MemoryDebug.h"

/**
 * @ingroup TcpStack
 * @brief Xml element 의 설정값들로 CFileLog 를 시작한다.
 * @param clsFileLog CFileLog 객체
 * @param clsElement Xml elment
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool SetupFileLog( CFileLog & clsFileLog, CXmlElement & clsElement )
{
	std::string strTemp;

	if( clsElement.SelectElementTrimData( "Folder", strTemp ) )
	{
		if( clsFileLog.Open( strTemp.c_str() ) == false )
		{
			CLog::Print( LOG_ERROR, "%s clsFileLog.Open(%s) error", __FUNCTION__, strTemp.c_str() );
			return false;
		}
	}

	int iLogLevel = 0;

	CXmlElement * pclsClient = clsElement.SelectElement( "Level" );
	if( pclsClient )
	{
		bool bTemp;

		pclsClient->SelectAttribute( "Debug", bTemp );
		if( bTemp ) iLogLevel |= LOG_DEBUG;

		pclsClient->SelectAttribute( "Info", bTemp );
		if( bTemp ) iLogLevel |= LOG_INFO;

		pclsClient->SelectAttribute( "Network", bTemp );
		if( bTemp ) iLogLevel |= LOG_NETWORK;

		pclsClient->SelectAttribute( "Sql", bTemp );
		if( bTemp ) iLogLevel |= LOG_SQL;
	}

	int			iLogMaxSize = 0;
	int64_t iLogMaxFolderSize = 0;

	clsElement.SelectElementData( "MaxSize", iLogMaxSize );
	clsElement.SelectElementData( "MaxFolderSize", iLogMaxFolderSize );

	clsFileLog.SetLevel( iLogLevel );
	clsFileLog.SetMaxLogSize( iLogMaxSize );
	clsFileLog.SetMaxFolderSize( iLogMaxFolderSize );

	return true;
}

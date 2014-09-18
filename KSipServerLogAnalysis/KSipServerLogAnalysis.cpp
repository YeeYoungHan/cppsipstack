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

#include "SipParserDefine.h"
#include "LogAnalysisSetup.h"
#include "LogFile.h"
#include "StatsSipMethod.h"
#include "StatsSipMethodIp.h"
#include "StatsSipMethodUserAgent.h"
#include "StatsSipReSend.h"
#include "TimeString.h"
#include "Directory.h"
#include "Log.h"
#include "Version.h"

/**
 * @ingroup KSipServerLogAnalysis
 * @brief 어제 날짜를 구한다.
 * @param strDate 어제 날짜를 저장할 변수
 */
void GetYesterday( std::string & strDate )
{
	struct tm	sttTm;
	time_t	iTime;
	char	szDate[9];

	time( &iTime );
	iTime -= 24 * 60 * 60;

#ifdef WIN32
	localtime_s( &sttTm, &iTime );
#else
	localtime_r( &iTime, &sttTm );	
#endif

	snprintf( szDate, sizeof(szDate), "%04d%02d%02d", sttTm.tm_year + 1900, sttTm.tm_mon + 1, sttTm.tm_mday );
	strDate = szDate;
}

/**
 * @ingroup KSipServerLogAnalysis
 * @brief 로그 파일을 분석한 후, 통계 정보에 저장한다.
 * @param pszFileName 로그 파일 이름
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool ReadLogFile( const char * pszFileName )
{
	char	szPacket[4096];
	CLogFile clsLogFile;
	CLogHeader clsLogHeader;

	if( clsLogFile.Open( pszFileName ) == false ) return false;

	memset( szPacket, 0, sizeof(szPacket) );
	while( clsLogFile.ReadSip( &clsLogHeader, szPacket, sizeof(szPacket) ) )
	{
		if( clsLogHeader.m_bSend == false )
		{
			CSipMessage clsMessage;

			if( clsMessage.Parse( szPacket, (int)strlen(szPacket) ) > 0 )
			{
				gclsStatsSipMethod.AddSipMessage( &clsMessage );
				gclsStatsSipMethodIp.AddSipMessage( &clsMessage, clsLogHeader.m_szIp );
				gclsStatsSipMethodUserAgent.AddSipMessage( &clsMessage );
				gclsStatsSipReSend.AddSipMessage( &clsMessage );
			}
			else
			{
				printf( "[ERROR] CSipMessage.Parse(%s)\n", szPacket );
			}
		}

		memset( szPacket, 0, sizeof(szPacket) );
	}

	clsLogFile.Close();
	gclsStatsSipReSend.Clear();

	return true;
}

/**
 * @ingroup KSipServerLogAnalysis
 * @brief main 함수
 * @param argc 
 * @param argv 
 * @returns 
 */
int main( int argc, char * argv[] )
{
	if( argc != 2 && argc != 3 )
	{
		printf( "%s (version - %s)\n", argv[0], PROGRAM_VERSION );
		printf( "[Usage] %s {setup xml filename} {date}\n", argv[0] );
		return -1;
	}

	if( gclsSetup.Read( argv[1] ) == false )
	{
		printf( "setup xml file read error\n" );
		return -1;
	}

	std::string strDate, strFileName;

	if( argc == 3 )
	{
		strDate = argv[2];

		if( !strcmp( strDate.c_str(), "all" ) )
		{

		}
		else if( !strcmp( strDate.c_str(), "today" ) )
		{
			char szDate[11];

			GetDateString( szDate, sizeof(szDate) );
			strDate = szDate;
		}
		else if( strDate.length() != 8 )
		{
			printf( "date(%s) is not correct\n", strDate.c_str() );
			return -1;
		}
	}
	else
	{
		GetYesterday( strDate );
	}

	if( !strcmp( strDate.c_str(), "all" ) )
	{
		FILE_LIST	clsFileList;
		FILE_LIST::iterator	itList;

		CDirectory::FileList( gclsSetup.m_strLogFolder.c_str(), clsFileList );
		CLog::SortFileList( clsFileList );

		for( itList = clsFileList.begin(); itList != clsFileList.end(); ++itList )
		{
			strFileName = gclsSetup.m_strLogFolder;
			CDirectory::AppendName( strFileName, itList->c_str() );
			ReadLogFile( strFileName.c_str() );
		}
	}
	else
	{
		strFileName = gclsSetup.m_strLogFolder;
		CDirectory::AppendName( strFileName, strDate.c_str() );

		strFileName.append( strDate );

		for( int i = 1; ; ++i )
		{
			char	szFileName[1024];
			
			snprintf( szFileName, sizeof(szFileName), "%s_%d.txt", strFileName.c_str(), i );
			if( ReadLogFile( szFileName ) == false ) break;
		}
	}

	gclsStatsSipMethod.SaveFile( strDate.c_str() );
	gclsStatsSipMethodIp.SaveFile( strDate.c_str() );
	gclsStatsSipMethodUserAgent.SaveFile( strDate.c_str() );

	return 0;
}

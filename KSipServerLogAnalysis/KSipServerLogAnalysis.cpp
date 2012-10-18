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
#include "SipMessage.h"
#include <time.h>

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

int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {setup xml filename}\n", argv[0] );
		return -1;
	}

	if( gclsSetup.Read( argv[1] ) == false )
	{
		return -1;
	}

	std::string strDate, strFileName;
	char	szFileName[1024], szPacket[4096];
	CLogFile clsLogFile;
	CLogHeader clsLogHeader;

	GetYesterday( strDate );
	strFileName = gclsSetup.m_strLogFolder;

#ifdef WIN32
	strFileName.append( "\\" );
#else
	strFileName.append( "/" );
#endif

	strFileName.append( strDate );

	for( int i = 1; ; ++i )
	{
		snprintf( szFileName, sizeof(szFileName), "%s_%d.txt", strFileName.c_str(), i );

		if( clsLogFile.Open( szFileName ) == false ) break;

		while( clsLogFile.ReadSip( &clsLogHeader, szPacket, sizeof(szPacket) ) )
		{
			CSipMessage clsMessage;

			clsMessage.Parse( szPacket, (int)strlen(szPacket) );
		}

		clsLogFile.Close();
	}

	return 0;
}

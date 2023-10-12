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

#include "SipLogAnalysis.h"

void ReadLogFile( const char * pszFileName )
{
	FILE * fd = fopen( pszFileName, "r" );
	if( fd == NULL )
	{
		printf( "file(%s) open error", pszFileName );
		return;
	}

	char szBuf[8192];
	char * pszPos;
	bool bSend;
	std::string strSipMessage;

	memset( szBuf, 0, sizeof(szBuf) );

	while( fgets( szBuf, sizeof(szBuf), fd ) )
	{
		if( strSipMessage.empty() == false )
		{
			if( szBuf[0] == ']' )
			{
				InsertSipMessage( strSipMessage, bSend );
				strSipMessage.clear();
			}
			else
			{
				int iLen = strlen( szBuf );

				if( szBuf[iLen-2] != '\r' && szBuf[iLen-1] == '\n' )
				{
					sprintf( szBuf + iLen - 1, "\r\n" );
				}

				strSipMessage.append( szBuf );
			}
		}
		else if( strstr( szBuf, "[fax] " ) )
		{
			ParseFaxLog( szBuf );
		}
		else
		{
			pszPos = strstr( szBuf, "UdpSend" );

			if( pszPos )
			{
				bSend = true;
			}
			else
			{
				bSend = false;
				pszPos = strstr( szBuf, "UdpRecv" );
			}

			if( pszPos )
			{
				int iLen = strlen( szBuf );

				if( szBuf[iLen-2] != '\r' && szBuf[iLen-1] == '\n' )
				{
					sprintf( szBuf + iLen - 1, "\r\n" );
				}

				pszPos = strstr( pszPos, "[" );
				if( pszPos )
				{
					strSipMessage = pszPos + 1;
				}
			}
		}

		memset( szBuf, 0, sizeof(szBuf) );
	}

	fclose( fd );
}

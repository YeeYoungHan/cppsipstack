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

#include "SipLogReassembly.h"

void ReadLogFile( FILE * in, FILE * out )
{
	char szBuf[8192];
	char * pszPos;
	int iLine = 0;
	std::string strSipMessage, strLog;
	bool bSend;

	memset( szBuf, 0, sizeof(szBuf) );

	while( fgets( szBuf, sizeof(szBuf), in ) )
	{
		++iLine;

		if( strSipMessage.empty() == false )
		{
			if( szBuf[0] == ']' )
			{
				CSipMessage clsMessage;
				std::string strCallId;

				if( clsMessage.Parse( strSipMessage.c_str(), strSipMessage.length() ) <= 0 )
				{
					printf( "line(%d) clsMessage.Parse(%s) error\n", iLine, strSipMessage.c_str() );
					break;
				}

				if( gclsPhoneMap.Select( clsMessage.m_clsFrom.m_clsUri.m_strUser.c_str() ) || gclsPhoneMap.Select( clsMessage.m_clsTo.m_clsUri.m_strUser.c_str() ) )
				{
					strLog.append( szBuf );
					fprintf( out, "%s", strLog.c_str() );
				}

				strSipMessage.clear();
				strLog.c_str();
			}
			else
			{
				int iLen = strlen( szBuf );

				if( szBuf[iLen-2] != '\r' && szBuf[iLen-1] == '\n' )
				{
					sprintf( szBuf + iLen - 1, "\r\n" );
				}

				strSipMessage.append( szBuf );
				strLog.append( szBuf );
			}
		}
		else
		{
			pszPos = strstr( szBuf, "UdpSend" );
			if( pszPos == NULL )
			{
				pszPos = strstr( szBuf, "TcpSend" );
			}

			if( pszPos )
			{
				bSend = true;
			}
			else
			{
				bSend = false;
				pszPos = strstr( szBuf, "UdpRecv" );
				if( pszPos == NULL )
				{
					pszPos = strstr( szBuf, "TcpRecv" );
				}
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
					strLog = szBuf;
				}
			}
		}

		memset( szBuf, 0, sizeof(szBuf) );
	}
}

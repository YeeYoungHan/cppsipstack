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

#include "SipLogSender.h"

void ReadLogFile( const char * pszFileName )
{
	FILE * fd = fopen( pszFileName, "r" );
	if( fd == NULL )
	{
		printf( "file(%s) open error", pszFileName );
		return;
	}

	char szBuf[8192], szIp[51], szSeq[101], szRecvSeq[101];
	char * pszPos;
	bool bSend;
	uint16_t sPort;
	int iBufSize, iLine = 0;
	std::string strSipMessage;

	memset( szBuf, 0, sizeof(szBuf) );

	while( fgets( szBuf, sizeof(szBuf), fd ) )
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

				clsMessage.m_clsCallId.ToString( strCallId );
				clsMessage.m_clsCSeq.ToString( szSeq, sizeof(szSeq) );

				if( bSend == false )
				{
					UdpSend( ghSocket, strSipMessage.c_str(), strSipMessage.length(), gpszSipIp, giSipPort );
				}
				else
				{
					iBufSize = sizeof(szBuf);
					UdpRecv( ghSocket, szBuf, &iBufSize, szIp, sizeof(szIp), &sPort );

					if( clsMessage.Parse( szBuf, iBufSize ) <= 0 )
					{
						printf( "line(%d) recv clsMessage.Parse(%s) error\n", iLine, strSipMessage.c_str() );
						break;
					}

					std::string strRecvCallId;

					clsMessage.m_clsCallId.ToString( strRecvCallId );
					clsMessage.m_clsCSeq.ToString( szRecvSeq, sizeof(szRecvSeq) );

					if( strcmp( strCallId.c_str(), strRecvCallId.c_str() ) )
					{
						
					}
				}

				// bSend = false 이면 SIP 서비스로 전송한다.
				// bSend = true 이면 SIP 서비스에서 수신할 때까지 대기한다.
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

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

#define RTP_PORT		"RtpPort("
#define FAX_RESULT	"end - result("

void ParseFaxLog( const char * pszLog )
{
	int iRtpPort = 0;
	std::string strErrorMessage;

	const char * pszPos = strstr( pszLog, RTP_PORT );
	if( pszPos == NULL ) return;

	pszPos += strlen( RTP_PORT );
	if( pszPos )
	{
		const char * pszStartPos = pszPos;
		std::string strPort;

		pszPos = strstr( pszStartPos, ")" );
		if( pszPos )
		{
			strPort.append( pszStartPos, pszPos - pszStartPos );
			iRtpPort = atoi( strPort.c_str() );
		}
	}

	pszPos = strstr( pszLog, FAX_RESULT );
	if( pszPos )
	{
		pszPos += strlen(FAX_RESULT);

		if( pszPos[0] == '0' )
		{
			// 성공
			gclsCallMap.UpdateWithRtpPort( iRtpPort, false, strErrorMessage );
		}
		else
		{
			// 실패
			pszPos = strstr( pszPos, "|" );
			if( pszPos )
			{
				const char * pszStartPos = pszPos + 1;

				pszPos = strstr( pszStartPos, ")" );
				if( pszPos )
				{
					strErrorMessage.append( pszStartPos, pszPos - pszStartPos );
					gclsCallMap.UpdateWithRtpPort( iRtpPort, true, strErrorMessage );
				}
			}
		}
	}
}

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
#include "SipMessage.h"
#include "SdpMessage.h"
#include "SipStatusCode.h"
#include "CallMap.h"

bool ParseSdp( std::string & strBody, CRtpInfo & clsInfo )
{
	CSdpMessage clsSdp;
	SDP_MEDIA_LIST::iterator itML;

	if( clsSdp.Parse( strBody.c_str(), strBody.length() ) > 0 )
	{
		clsInfo.m_strIp = clsSdp.m_clsConnection.m_strAddr;

		for( itML = clsSdp.m_clsMediaList.begin(); itML != clsSdp.m_clsMediaList.end(); ++itML )
		{
			if( !strcmp( itML->m_strMedia.c_str(), "audio" ) )
			{
				clsInfo.m_iPort = itML->m_iPort;
				return true;
			}
		}
	}

	return false;
}

bool InsertSipMessage( std::string & strSipMessage, bool bSend )
{
	CSipMessage clsMessage;
	CCallInfo clsInfo;
	std::string strCallId;

	if( clsMessage.Parse( strSipMessage.c_str(), strSipMessage.length() ) > 0 )
	{
		clsMessage.m_clsCallId.ToString( strCallId );

		if( clsMessage.IsRequest() )
		{
			if( clsMessage.IsMethod( "INVITE" ) )
			{
				clsInfo.m_bSend = bSend;
				clsInfo.m_strFrom = clsMessage.m_clsFrom.m_clsUri.m_strUser;
				clsInfo.m_strTo = clsMessage.m_clsTo.m_clsUri.m_strUser;
				clsInfo.m_strCallId = strCallId;

				ParseSdp( clsMessage.m_strBody, clsInfo.m_clsFromRtp );

				gclsCallMap.Insert( clsInfo );
			}
			else if( clsMessage.IsMethod( "BYE" ) )
			{
				gclsCallMap.Delete( strCallId );
			}
		}
		else
		{
			if( clsMessage.IsMethod( "INVITE" ) )
			{
				if( clsMessage.m_iStatusCode == SIP_OK )
				{
					if( gclsCallMap.Select( strCallId, clsInfo ) )
					{
						if( ParseSdp( clsMessage.m_strBody, clsInfo.m_clsToRtp ) )
						{
							gclsCallMap.Update( clsInfo );
						}
					}
				}
				else if( clsMessage.m_iStatusCode >= 300 )
				{
					gclsCallMap.Delete( strCallId );
				}
			}
		}

		return true;
	}

	return false;
}

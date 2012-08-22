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

bool GetCallIdFromReferTo( const char * pszValue, std::string & strReferToCallId )
{
	const char * pszPos, * pszNext;

	strReferToCallId.clear();

	pszPos = strstr( pszValue, "Replaces=" );
	if( pszPos == NULL ) return false;
	pszPos += 9;

	pszNext = strstr( pszPos, "%3B" );
	if( pszNext == NULL )
	{
		pszNext = strstr( pszPos, "%3b" );
		if( pszNext == NULL )	return false;
	}

	strReferToCallId.append( pszPos, pszNext - pszPos );
	ReplaceString( strReferToCallId, "%40", "@" );

	return true;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP REFER 요청 메시지 수신 이벤트 핸들러
 * @param iThreadId		SIP stack 의 UDP 쓰레드 아이디
 * @param pclsMessage 수신된 SIP 요청 메시지
 * @returns 정상적으로 처리하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::RecvReferRequest( int iThreadId, CSipMessage * pclsMessage )
{
	CSipMessage * pclsResponse;
	std::string	strCallId;
	SIP_DIALOG_MAP::iterator	itMap;
	bool	bFound = false;

	pclsMessage->GetCallId( strCallId );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strCallId );
	if( itMap != m_clsMap.end() )
	{
		bFound = true;
	}
	m_clsMutex.release();

	if( bFound == false )
	{
		pclsResponse = pclsMessage->CreateResponse( SIP_CALL_TRANSACTION_DOES_NOT_EXIST );
	}
	else
	{
		CSipHeader * pclsHeader = pclsMessage->GetHeader( "Refer-To" );
		if( pclsHeader == NULL )
		{
			// RFC-3515 : An agent responding to a REFER method MUST return a 400 (Bad Request) if the request contained zero or more than one Refer-To header field values.
			pclsResponse = pclsMessage->CreateResponse( SIP_BAD_REQUEST );
		}
		else
		{
			std::string	strReferToCallId;

			if( GetCallIdFromReferTo( pclsHeader->m_strValue.c_str(), strReferToCallId ) == false )
			{
				// QQQ: Blind Transfer
			}
			else
			{
				bool bScreenedTransfer = true;
				bFound = false;

				m_clsMutex.acquire();
				itMap = m_clsMap.find( strReferToCallId );
				if( itMap != m_clsMap.end() )
				{
					if( itMap->second.m_sttStartTime.tv_sec == 0 )
					{
						bScreenedTransfer = false;
					}
					bFound = true;
				}
				m_clsMutex.release();

				if( bFound == false )
				{
					pclsResponse = pclsMessage->CreateResponse( SIP_NOT_FOUND );
				}
				else
				{
					if( m_pclsCallBack )
					{
						if( m_pclsCallBack->EventTransfer( strCallId.c_str(), strReferToCallId.c_str(), bScreenedTransfer ) )
						{
							pclsResponse = pclsMessage->CreateResponse( SIP_ACCEPTED );
						}
						else
						{
							pclsResponse = pclsMessage->CreateResponse( SIP_NOT_FOUND );
						}
					}
				}
			}
		}
	}

	if( pclsResponse )
	{
		gclsSipStack.SendSipMessage( pclsResponse );
		return true;
	}

	return false;
}

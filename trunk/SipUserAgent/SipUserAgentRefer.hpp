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

/**
 * @ingroup SipUserAgent
 * @brief Replaces �� ������ ���ڿ����� SIP Call-ID �� �����´�.
 * @param pszValue	Replaces �� ������ ���ڿ�
 * @param strReferToCallId SIP Call-ID �� ������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief SIP REFER ��û �޽��� ���� �̺�Ʈ �ڵ鷯
 * @param iThreadId		SIP stack �� UDP ������ ���̵�
 * @param pclsMessage ���ŵ� SIP ��û �޽���
 * @returns ���������� ó���ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::RecvReferRequest( int iThreadId, CSipMessage * pclsMessage )
{
	CSipMessage * pclsResponse = NULL;
	std::string	strCallId;
	SIP_DIALOG_MAP::iterator	itMap;
	bool	bFound = false;

	if( pclsMessage->GetCallId( strCallId ) == false )
	{
		m_clsSipStack.SendSipMessage( pclsMessage->CreateResponse( SIP_BAD_REQUEST ) );
		return true;
	}

	if( m_pclsCallBack )
	{
		if( m_pclsCallBack->EventIncomingRequestAuth( pclsMessage ) == false )
		{
			return true;
		}
	}

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( strCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		bFound = true;
	}
	m_clsDialogMutex.release();

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
				// Blind Transfer
				CSipUri	clsReferToUri;

				if( clsReferToUri.Parse( pclsHeader->m_strValue.c_str(), (int)pclsHeader->m_strValue.length() ) == -1 )
				{
					pclsResponse = pclsMessage->CreateResponse( SIP_BAD_REQUEST );
				}
				else
				{
					if( m_pclsCallBack )
					{
						if( m_pclsCallBack->EventBlindTransfer( strCallId.c_str(), clsReferToUri.m_strUser.c_str() ) )
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
			else
			{
				// Screened / Unscreened Transfer
				bool bScreenedTransfer = true;
				bFound = false;

				m_clsDialogMutex.acquire();
				itMap = m_clsDialogMap.find( strReferToCallId );
				if( itMap != m_clsDialogMap.end() )
				{
					if( itMap->second.m_sttStartTime.tv_sec == 0 )
					{
						bScreenedTransfer = false;
					}
					bFound = true;
				}
				m_clsDialogMutex.release();

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
		m_clsSipStack.SendSipMessage( pclsResponse );
		return true;
	}

	return false;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP REFER ���� �޽��� ���� �̺�Ʈ �ڵ鷯
 * @param iThreadId		SIP stack �� UDP ������ ���̵�
 * @param pclsMessage ���ŵ� SIP ���� �޽���
 * @returns ���������� ó���ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::RecvReferResponse( int iThreadId, CSipMessage * pclsMessage )
{
	std::string	strCallId;
	SIP_DIALOG_MAP::iterator	itMap;
	bool	bFound = false;

	if( pclsMessage->GetCallId( strCallId ) == false )
	{
		return true;
	}

	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( strCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		bFound = true;
	}
	m_clsDialogMutex.release();

	if( bFound )
	{
		if( m_pclsCallBack )
		{
			m_pclsCallBack->EventTransferResponse( strCallId.c_str(), pclsMessage->m_iStatusCode );
		}
		return true;
	}

	return false;
}

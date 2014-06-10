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
 * @brief SIP PRACK ��û �޽��� ���� �̺�Ʈ �ڵ鷯
 * @param iThreadId		SIP stack �� UDP ������ ���̵�
 * @param pclsMessage ���ŵ� SIP ��û �޽���
 * @returns ���������� ó���ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::RecvPrackRequest( int iThreadId, CSipMessage * pclsMessage )
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
		pclsResponse = pclsMessage->CreateResponse( SIP_OK );
	}

	if( pclsResponse )
	{
		m_clsSipStack.SendSipMessage( pclsResponse );

		if( m_pclsCallBack )
		{
			CSipCallRtp clsRtp;
			bool bSdp = false;

			if( GetSipCallRtp( pclsMessage, clsRtp ) )
			{
				bSdp = true;
			}

			m_pclsCallBack->EventPrack( strCallId.c_str(), bSdp ? &clsRtp : NULL );
		}

		return true;
	}

	return false;
}

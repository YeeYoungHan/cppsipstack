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
 * @brief SIP OPTIONS ��û �޽��� ���� �̺�Ʈ �ڵ鷯
 * @param iThreadId		SIP stack �� UDP ������ ���̵�
 * @param pclsMessage ���ŵ� SIP ��û �޽���
 * @returns ���������� ó���ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipUserAgent::RecvOptionsRequest( int iThreadId, CSipMessage * pclsMessage )
{
	CSipMessage * pclsResponse = NULL;
	std::string	strCallId;
	SIP_DIALOG_MAP::iterator	itMap;

	if( pclsMessage->GetCallId( strCallId ) == false )
	{
		m_clsSipStack.SendSipMessage( pclsMessage->CreateResponse( SIP_BAD_REQUEST ) );
		return true;
	}

	pclsResponse = pclsMessage->CreateResponse( SIP_OK );

	if( pclsResponse )
	{
		m_clsSipStack.SendSipMessage( pclsResponse );
		return true;
	}

	return false;
}

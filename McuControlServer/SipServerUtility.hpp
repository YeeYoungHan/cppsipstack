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

void CSipServer::UpdateBranch( CSipMessage * pclsMessage, std::string & strBranch )
{
	pclsMessage->m_strUserAgent = SIP_USER_AGENT;
	pclsMessage->MakePacket();

	// PolyCom 에서 전송한 패킷이 flagment 되었고 flagment 된 패킷의 길이가 60 미만인 경우
	// 상대방으로 전달되지 않는 버그를 패치하는 기능
	int iLength = pclsMessage->m_strPacket.length();
	if( iLength > 1472 && iLength < 1498 )
	{
		int iAppendLength = 1498 - iLength;

		for( int i = 0; i < iAppendLength; ++i )
		{
			strBranch.append( "1" );
		}

		SIP_VIA_LIST::iterator itViaList = pclsMessage->m_clsViaList.begin();
		
		UpdateSipParameter( itViaList->m_clsParamList, "branch", strBranch.c_str() );
	}
}

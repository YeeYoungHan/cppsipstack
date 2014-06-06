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
 * @ingroup SipSpeed
 * @brief SIP REGISTER 응답 메시지 수신 이벤트 핸들러
 * @param pclsInfo	SIP REGISTER 응답 메시지를 전송한 IP-PBX 정보 저장 객체
 * @param iStatus		SIP REGISTER 응답 코드
 */
void CSipSpeedDlg::_EventRegister( CSipServerInfo * pclsInfo, int iStatus )
{
	std::string	strCommand;

	if( pclsInfo->m_bLogin )
	{
		strCommand = "login";
	}
	else
	{
		strCommand = "logout";
	}

	if( !strcmp( pclsInfo->m_strUserId.c_str(), m_strCallerId ) )
	{
		if( iStatus == SIP_OK )
		{
			if( pclsInfo->m_bLogin )
			{
				m_bCallerLogin = true;
			}
			else
			{
				m_bCallerLogin = false;
			}

			SetLog( "Caller(%s) %s success", m_strCallerId, strCommand.c_str() );
		}
		else
		{
			m_bCallerLogin = false;
			SetLog( "Caller(%s) %s error(%d)", m_strCallerId, strCommand.c_str(), iStatus );
		}
	}
	else if( !strcmp( pclsInfo->m_strUserId.c_str(), m_strCalleeId ) )
	{
		if( iStatus == SIP_OK )
		{
			if( pclsInfo->m_bLogin )
			{
				m_bCalleeLogin = true;
			}
			else
			{
				m_bCalleeLogin = false;
			}

			SetLog( "Callee(%s) %s success", m_strCalleeId, strCommand.c_str() );
		}
		else
		{
			m_bCalleeLogin = false;
			SetLog( "Callee(%s) %s error(%d)", m_strCalleeId, strCommand.c_str(), iStatus );
		}
	}

	// Caller, Callee 가 모두 로그인되면 테스트를 시작할 수 있다.
	if( pclsInfo->m_bLogin && m_bCallerLogin && m_bCalleeLogin )
	{
		if( m_bTest == false )
		{
			m_btnStartTest.EnableWindow( TRUE );
		}
	}
}

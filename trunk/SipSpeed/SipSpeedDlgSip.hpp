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

LRESULT CSipSpeedDlg::OnSipMessage( WPARAM wParam, LPARAM lParam )
{
	return m_clsSipUserAgentMFC.OnSipMessage( wParam, lParam );
}

/**
 * @ingroup SipSpeed
 * @brief SIP REGISTER ���� �޽��� ���� �̺�Ʈ �ڵ鷯
 * @param pclsInfo	SIP REGISTER ���� �޽����� ������ IP-PBX ���� ���� ��ü
 * @param iStatus		SIP REGISTER ���� �ڵ�
 */
void CSipSpeedDlg::EventRegister( CSipServerInfo * pclsInfo, int iStatus )
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

	// Caller, Callee �� ��� �α��εǸ� �׽�Ʈ�� ������ �� �ִ�.
	if( pclsInfo->m_bLogin && m_bCallerLogin && m_bCalleeLogin )
	{
		if( m_bTest == false )
		{
			m_btnStartTest.EnableWindow( TRUE );
		}
	}
}

/**
 * @ingroup SipSpeed
 * @brief SIP ��ȭ ��û ���ſ� ���� ���� Ȯ�� �̺�Ʈ �ڵ鷯
 * @param pclsMessage	SIP INVITE ��û �޽���
 * @return ������ �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipSpeedDlg::EventIncomingRequestAuth( CSipMessage * pclsMessage )
{
	return true;
}

/**
 * @ingroup SipSpeed
 * @brief SIP ��ȭ ��û ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId	SIP Call-ID
 * @param pszFrom		SIP From ����� ���̵�
 * @param pszTo			SIP To ����� ���̵�
 * @param pclsRtp		RTP ���� ���� ��ü
 */
void CSipSpeedDlg::EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp )
{
	CSipCallRtp clsRtp;

	clsRtp.m_strIp = gclsSipUserAgent.m_clsSipStack.m_clsSetup.m_strLocalIp;
	clsRtp.m_iPort = 4002;	
	clsRtp.m_iCodec = 0;

	gclsSipUserAgent.AcceptCall( pszCallId, &clsRtp );
}

/**
 * @ingroup SipSpeed
 * @brief SIP Ring / Session Progress ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId		SIP Call-ID
 * @param iSipStatus	SIP ���� �ڵ�
 * @param pclsRtp			RTP ���� ���� ��ü
 */
void CSipSpeedDlg::EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp )
{
}

/**
 * @ingroup SipSpeed
 * @brief SIP ��ȭ ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId	SIP Call-ID
 * @param pclsRtp		RTP ���� ���� ��ü
 */
void CSipSpeedDlg::EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	m_clsMutex.acquire();
	++m_iCallSuccess;
	SetPercent( );
	m_clsMutex.release();

	gclsSipUserAgent.StopCall( pszCallId );
}

/**
 * @ingroup SipSpeed
 * @brief SIP ��ȭ ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId		SIP Call-ID
 * @param iSipStatus	SIP ���� �ڵ�. INVITE �� ���� ���� �������� ��ȭ�� ����� ���, INVITE �� ���� �ڵ带 �����Ѵ�.
 */
void CSipSpeedDlg::EventCallEnd( const char * pszCallId, int iSipStatus )
{
	if( iSipStatus != SIP_OK )
	{
		m_clsMutex.acquire();
		++m_iCallError;
		SetPercent( );
		m_clsMutex.release();
	}
}

/**
 * @ingroup SipSpeed
 * @brief SIP ReINVITE ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId	SIP Call-ID
 * @param pclsRtp		RTP ���� ���� ��ü
 */
void CSipSpeedDlg::EventReInvite( const char * pszCallId, CSipCallRtp * pclsRtp )
{
}

/**
 * @ingroup SipSpeed
 * @brief Screened / Unscreened Transfer ��û ���� �̺�Ʈ �ڵ鷯
 * @param pszCallId					SIP Call-ID
 * @param pszReferToCallId	��ȭ�� ���޵� SIP Call-ID
 * @param bScreenedTransfer Screened Transfer �̸� true �� �Էµǰ� Unscreened Transfer �̸� false �� �Էµȴ�.
 * @returns ��û�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipSpeedDlg::EventTransfer( const char * pszCallId, const char * pszReferToCallId, bool bScreenedTransfer )
{
	return false;
}

/**
 * @ingroup SipSpeed
 * @brief Blind Transfer ��û ���� �̺�Ʈ �ڵ鷯
 * @param pszCallId			SIP Call-ID
 * @param pszReferToId	��ȭ�� ���޵� ����� ���̵�
 * @returns ��û�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipSpeedDlg::EventBlindTransfer( const char * pszCallId, const char * pszReferToId )
{
	return false;
}

/**
 * @ingroup SipSpeed
 * @brief SIP MESSAGE ���� �̺�Ʈ �ڵ鷯
 * @param pszFrom			SIP �޽��� ���� ���̵�
 * @param pszTo				SIP �޽��� ���� ���̵�
 * @param pclsMessage SIP �޽���
 * @returns ��û�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipSpeedDlg::EventMessage( const char * pszFrom, const char * pszTo, CSipMessage * pclsMessage )
{
	return false;
}

/**
 * @ingroup SipSpeed
 * @brief SIP ��û �޽��� ���� �̺�Ʈ �ڵ鷯
 * @param iThreadId		UDP ������ ��ȣ
 * @param pclsMessage SIP ��û �޽���
 * @returns SIP ��û �޽����� ó���ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipSpeedDlg::RecvRequest( int iThreadId, CSipMessage * pclsMessage )
{
	return false;
}

/**
 * @ingroup SipSpeed
 * @brief SIP ���� �޽��� ���� �̺�Ʈ �ڵ鷯
 * @param iThreadId		UDP ������ ��ȣ
 * @param pclsMessage SIP ���� �޽���
 * @returns SIP ���� �޽����� ó���ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipSpeedDlg::RecvResponse( int iThreadId, CSipMessage * pclsMessage )
{
	if( pclsMessage->IsMethod( "OPTIONS" ) )
	{
		std::string strCallId;

		pclsMessage->GetCallId( strCallId );

		if( gclsCallIdMap.Delete( strCallId.c_str() ) )
		{
			m_clsMutex.acquire();
			if( pclsMessage->m_iStatusCode == SIP_REQUEST_TIME_OUT )
			{
				++m_iCallError;
			}
			else
			{
				++m_iCallSuccess;
			}
			SetPercent( );
			m_clsMutex.release();
		}

		return true;
	}

	return false;
}

/**
 * @ingroup SipSpeed
 * @brief SIP �޽��� ���� timeout �̺�Ʈ �ڵ鷯
 * @param iThreadId		UDP ������ ��ȣ
 * @param pclsMessage SIP ���� �޽���
 * @returns SIP ���� �޽����� ó���ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipSpeedDlg::SendTimeout( int iThreadId, CSipMessage * pclsMessage )
{
	return false;
}

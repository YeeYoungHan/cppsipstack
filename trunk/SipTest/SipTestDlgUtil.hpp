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
 * @ingroup SipTest
 * @brief �Է°��� ��ȿ���� �˻��Ѵ�.
 * @param strInput	�Է°�
 * @param pszName		�Է� �̸�
 * @returns �Է°��� ��ȿ�ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipTestDlg::CheckInput( CString & strInput, const char * pszName )
{
	if( strInput.IsEmpty() )
	{
		CString	strText;

		strText.Format( "Please ~ insert %s", pszName ); 

		MessageBox( strText, "Input Error", MB_OK | MB_ICONERROR );
		return false;
	}

	return true;
}

/**
 * @brief �α׸� �����Ѵ�.
 * @param fmt �α� �޽���
 */
void CSipTestDlg::SetLog( const char * fmt, ... )
{
	va_list		ap;
	char			szBuf[8192], szTime[51];
	SYSTEMTIME	sttTime;

	GetLocalTime( &sttTime );

	snprintf( szTime, sizeof(szTime), "[%02d:%02d:%02d.%03d] ", sttTime.wHour, sttTime.wMinute, sttTime.wSecond, sttTime.wMilliseconds );

	va_start( ap, fmt );
	vsnprintf( szBuf, sizeof(szBuf)-1, fmt, ap );
	va_end( ap );

	m_clsMutex.acquire();

	UpdateData(TRUE);
	m_strLog.Append( szTime );
	m_strLog.Append( szBuf );
	m_strLog.Append( "\r\n" );
	UpdateData(FALSE);

	int iLine = m_txtLog.GetLineCount();
	m_txtLog.LineScroll( iLine );

	m_clsMutex.release();
}

/**
 * @brief �׽�Ʈ ���� ���¸� �����ش�.
 */
void CSipTestDlg::SetPercent( )
{
	int iPercent = 0;

	m_clsProgress.SetPos( iPercent );

	m_strPercent.Format( "%d %%", iPercent );
	UpdateData(FALSE);
}

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
 * @brief 입력값이 유효한지 검사한다.
 * @param strInput	입력값
 * @param pszName		입력 이름
 * @returns 입력값이 유효하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
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
 * @brief 로그를 저장한다.
 * @param fmt 로그 메시지
 */
void CSipTestDlg::SetLog( const char * fmt, ... )
{
	va_list		ap;
	char			szBuf[8192];

	va_start( ap, fmt );
	vsnprintf( szBuf, sizeof(szBuf)-1, fmt, ap );
	va_end( ap );

	m_clsMutex.acquire();

	UpdateData(TRUE);
	m_strLog.Append( szBuf );
	m_strLog.Append( "\r\n" );
	UpdateData(FALSE);

	int iLine = m_txtLog.GetLineCount();
	m_txtLog.LineScroll( iLine );

	m_clsMutex.release();
}

/**
 * @brief 테스트 진행 상태를 보여준다.
 */
void CSipTestDlg::SetPercent( )
{
	int iPercent = 0;

	m_clsProgress.SetPos( iPercent );

	m_strPercent.Format( "%d %%", iPercent );
	UpdateData(FALSE);
}

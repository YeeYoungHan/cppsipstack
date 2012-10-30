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

#include "SipStackDefine.h"
#include "SipBuffer.h"
#include <string.h>
#include <stdlib.h>

/** 내부 변수를 초기화시킨다. */ 
CSipBuffer::CSipBuffer() : m_pszBuf(NULL), m_iBufLen(0)
{
}

CSipBuffer::~CSipBuffer()
{
	if( m_pszBuf ) delete [] m_pszBuf;
}

/** 내부 버퍼에 데이타를 추가한다. 
 *
 *	@param	pszBuf	입력할 데이타
 *	@param	iLen		입력할 데이타의 길이
 *	@return	성공하면 true 를 리턴한다. 실패하면 false 를 리턴한다.
 */
bool CSipBuffer::AddBuf( const char * pszBuf, int iLen )
{
	if( m_pszBuf == NULL )
	{
		m_pszBuf = new char[SIP_MAX_BUF_SIZE];
		if( m_pszBuf == NULL )
		{
			return false;
		}

		memset( m_pszBuf, 0, SIP_MAX_BUF_SIZE );
	}

	if( m_iBufLen + iLen >= SIP_MAX_BUF_SIZE )
	{
		return false;
	}

	memcpy( m_pszBuf + m_iBufLen, pszBuf, iLen );
	m_iBufLen += iLen;

	return true;
}

/** 내부 버퍼에서 SIP 메시지를 추출한다. 
 *
 *	@param	ppszBuf	추출한 SIP 메시지의 포인터를 저장할 변수
 *	@param	piLen		추출한 SIP 메시지의 길이
 *	@return	성공하면 true 를 리턴한다. 실패하면 false 를 리턴한다.
 */
bool CSipBuffer::GetSipMessage( char ** ppszBuf, int * piLen )
{
	int		i, iLen = 0, iContentLength = -1, iBodyLength = 0;
	bool	bIsBodyStart = false;
	bool	bIsContentLengthRow = false;
	char	szLength[22];
	int		iLengthPos = 0;
	static const char * pszContentLength = "Content-Length";
	static int	iContentLengthLen = (int)strlen(pszContentLength);

	*ppszBuf = NULL;
	*piLen = 0;

	// 맨 앞의 공백은 무시한다.
	for( i = 0; i < m_iBufLen; ++i )
	{
		if( m_pszBuf[i] != ' ' && m_pszBuf[i] != '\t' && m_pszBuf[i] != '\r' && m_pszBuf[i] != '\n' )
		{
			break;
		}
	}

	if( i > 0 )
	{
		ShiftBuf( i );
	}

	for( i = 0; i < m_iBufLen; ++i )
	{
		if( bIsBodyStart )
		{
			++iBodyLength;
			if( iContentLength == iBodyLength )
			{
				*ppszBuf = m_pszBuf;
				*piLen = i+1;
				break;
			}
		}
		else
		{
			++iLen;

			if( m_pszBuf[i] == '\n' )
			{
				if( i >= 1 && m_pszBuf[i-1] == '\r' )
				{
					if( iContentLength == -1 && bIsContentLengthRow )
					{
						szLength[iLengthPos] = '\0';
						iContentLength = atoi(szLength);
						bIsContentLengthRow = false;
					}
					else if( i >= 3 && m_pszBuf[i-3] == '\r' && m_pszBuf[i-2] == '\n' )
					{
						if( iContentLength == 0 )
						{
							*ppszBuf = m_pszBuf;
							*piLen = i+1;
							break;
						}
						bIsBodyStart = true;
					}
				}
				iLen = 0;
				bIsContentLengthRow = false;
			}
			else
			{
				if( bIsContentLengthRow )
				{
					if( m_pszBuf[i] != ':' && m_pszBuf[i] != ' ' && m_pszBuf[i] != '\r' )
					{
						if( iLengthPos < (int)(sizeof(szLength)-1) )
						{
							szLength[iLengthPos++] = m_pszBuf[i];
						}
					}
				}
				else if( iContentLength == -1 && iLen == iContentLengthLen )
				{
					if( !strncmp( m_pszBuf + i - (iLen-1), pszContentLength, iContentLengthLen ) )
					{
						bIsContentLengthRow = true;
					}
				}
			}
		}
	}

	if( *piLen == 0 ) return false;

	return true;
}

/** 내부 버퍼를 왼쪽으로 이동한다.
 *	- SIP 메시지를 처리한 후에 호출하면 된다.
 *
 *	@param	iLen	이동할 길이
 *	@return	성공하면 true 를 리턴한다. 실패하면 false 를 리턴한다.
 */
bool CSipBuffer::ShiftBuf( int iLen )
{
	if( iLen > m_iBufLen ) return false;

	memcpy( m_pszBuf, m_pszBuf + iLen, m_iBufLen - iLen );
	m_iBufLen -= iLen;

	return true;
}

/**
 * @brief 내부 변수를 초기화시킨다.
 */
void CSipBuffer::Clear()
{
	if( m_pszBuf )
	{
		delete [] m_pszBuf;
		m_pszBuf = NULL;
	}

	m_iBufLen = 0;
}

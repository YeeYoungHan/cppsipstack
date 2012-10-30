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

#ifndef _SIP_BUFFER_H_
#define _SIP_BUFFER_H_

#define SIP_MAX_BUF_SIZE	8192

#include "SipUdp.h"

/** SIP TCP 버퍼를 관리하는 클래스 */
class CSipBuffer
{
public:
	CSipBuffer();
	~CSipBuffer();

	bool AddBuf( const char * pszBuf, int iLen );
	bool GetSipMessage( char ** ppszBuf, int * piLen );
	bool ShiftBuf( int iLen );
	void Clear();

private:
	char		* m_pszBuf;
	int			m_iBufLen;
};

#endif

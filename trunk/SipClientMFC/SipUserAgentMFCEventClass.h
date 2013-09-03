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

#ifndef _SIP_USER_AGENT_MFC_EVENT_CLASS_H_
#define _SIP_USER_AGENT_MFC_EVENT_CLASS_H_

class CEventRegister
{
public:
	CEventRegister( CSipServerInfo * pclsInfo, int iStatus )
	{
		m_clsInfo = *pclsInfo;
		m_iStatus = iStatus;
	}

	CSipServerInfo m_clsInfo;
	int		m_iStatus;
};

class CEventIncomingCall
{
public:
	CEventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp )
	{
		m_pszCallId = pszCallId;
		m_pszFrom = pszFrom;
		m_pszTo = pszTo;
		m_pclsRtp = pclsRtp;
	}

	const char * m_pszCallId;
	const char * m_pszFrom;
	const char * m_pszTo;
	CSipCallRtp * m_pclsRtp;
};

#endif

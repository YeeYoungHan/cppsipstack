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

#include "StatsSipReSend.h"

CStatsSipReSend gclsStatsSipReSend;

CStatsSipReSend::CStatsSipReSend()
{
}

CStatsSipReSend::~CStatsSipReSend()
{
}

void CStatsSipReSend::AddSipMessage( CSipMessage * pclsMessage )
{
	SIP_SEND_MAP::iterator	itMap;
	std::string	strKey;

	GetKey( pclsMessage, strKey );

	itMap = m_clsMap.find( strKey );
	if( itMap == m_clsMap.end() )
	{
		CSipSendInfo clsInfo;

		m_clsMap.insert( SIP_SEND_MAP::value_type( strKey, clsInfo ) );
	}
	else
	{
		printf( "resend(%s)\n", strKey.c_str() );
	}
}

void CStatsSipReSend::Clear()
{
	m_clsMap.clear();
}

void CStatsSipReSend::GetKey( CSipMessage * pclsMessage, std::string & strKey )
{
	std::string strCallId;
	char szSeq[101];

	strKey.clear();

	pclsMessage->GetCallId( strCallId );
	pclsMessage->m_clsCSeq.ToString( szSeq, sizeof(szSeq) );

	if( pclsMessage->IsRequest() )
	{
		strKey.append( "req" );
	}
	else
	{
		strKey.append( "res" );
	}

	strKey.append( "_" );
	strKey.append( strCallId );
	strKey.append( "_" );
	strKey.append( szSeq );
}

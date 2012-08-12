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

#ifndef ANDROID
#include "SipParserDefine.h"
#else
#include "../SipParser/SipParserDefine.h"
#endif
#include "SdpTime.h"

CSdpTime::CSdpTime()
{
}

CSdpTime::~CSdpTime()
{
}

int CSdpTime::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int		iPos, iStartPos = 0, iType = 0;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == ' ' )
		{
			SetData( pszText + iStartPos, iPos - iStartPos, iType );
			++iType;
			iStartPos = iPos + 1;
		}
	}

	if( iStartPos < iTextLen )
	{
		SetData( pszText + iStartPos, iPos - iStartPos, iType );
		++iType;
	}

	if( iType != 2 ) return -1;

	return iTextLen;
}

int CSdpTime::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;
	if( Empty() ) return -1;

	int iLen;

	iLen = snprintf( pszText, iTextSize, "%s %s\r\n", m_strStartTime.c_str(), m_strStopTime.c_str() );

	if( m_clsRepeatTimeList.size() > 0 )
	{
		SDP_REPEAT_TIME_LIST::iterator	itRTL;

		for( itRTL = m_clsRepeatTimeList.begin(); itRTL != m_clsRepeatTimeList.end(); ++itRTL )
		{
			iLen = snprintf( pszText, iTextSize, "r=%s\r\n", itRTL->c_str() );
		}
	}

	return iLen;
}

void CSdpTime::Clear()
{
	m_strStartTime.clear();
	m_strStopTime.clear();
	m_clsRepeatTimeList.clear();
}

bool CSdpTime::Empty()
{
	if( m_strStartTime.empty() || m_strStopTime.empty() ) return true;

	return false;
}

void CSdpTime::SetData( const char * pszData, int iLen, int iType )
{
	switch( iType )
	{
	case 0:
		m_strStartTime.append( pszData, iLen );
		break;
	case 1:
		m_strStopTime.append( pszData, iLen );
		break;
	}
}

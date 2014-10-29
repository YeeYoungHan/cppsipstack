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

#include "SipParserDefine.h"
#include "SipVia.h"
#include <stdlib.h>
#include "SipUtility.h"
#include "MemoryDebug.h"

CSipVia::CSipVia() : m_iPort(-1)
{
}

CSipVia::~CSipVia()
{
}

/**
 * @ingroup SipParser
 * @brief SIP ��� ���ڿ��� �Ľ��Ͽ� CSipVia Ŭ������ ��� ������ �����Ѵ�.
 * @param pszText		SIP ����� ���� ������ ���ڿ�
 * @param iTextLen	pszText ���ڿ��� ����
 * @returns �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipVia::Parse( const char * pszText, int iTextLen )
{
	if( pszText == NULL || iTextLen <= 0 ) return -1;
	Clear();

	int iPos, iCurPos = 0;

	iPos = ParseSentProtocol( pszText, iTextLen );
	if( iPos == -1 ) return -1;
	iCurPos += iPos;

	iPos = ParseSentBy( pszText + iCurPos, iTextLen - iCurPos );
	if( iPos == -1 ) return -1;
	iCurPos += iPos;

	iPos = HeaderListParamParse( pszText + iCurPos, iTextLen - iCurPos );
	if( iPos == -1 ) return -1;
	iCurPos += iPos;

	return iCurPos;
}

/**
 * @ingroup SipParser
 * @brief SIP �޽����� ���Ե� ���ڿ��� �ۼ��Ѵ�.
 * @param pszText		SIP ����� ���� ������ ���ڿ� ����
 * @param iTextSize	pszText ������ ũ��
 * @returns �����ϸ� �ۼ��� ���ڿ� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipVia::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;
	if( m_strProtocolName.empty() || m_strProtocolVersion.empty() || m_strTransport.empty() || m_strHost.empty() ) return 0;

	int iLen = 0, iPos;

	iLen += snprintf( pszText, iTextSize, "%s/%s/%s ", m_strProtocolName.c_str(), m_strProtocolVersion.c_str(), m_strTransport.c_str() );

	iLen += SipIpv6Print( m_strHost, pszText, iTextSize, iLen );

	if( m_iPort > 0 )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, ":%d", m_iPort );
	}

	iPos = ParamToString( pszText + iLen, iTextSize - iLen );
	if( iPos == -1 ) return -1;
	iLen += iPos;

	return iLen;
}

/**
 * @ingroup SipParser
 * @brief ��� ������ �ʱ�ȭ��Ų��.
 */
void CSipVia::Clear()
{
	m_strProtocolName.clear();
	m_strProtocolVersion.clear();
	m_strTransport.clear();
	m_strHost.clear();
	m_iPort = -1;

	ClearParam();
}

/**
 * @brief ���ڿ����� �������� �� �������� ������ �Ľ��Ѵ�.
 * @param pszText		SIP ����� ���� ������ ���ڿ�
 * @param iTextLen	pszText ���ڿ��� ����
 * @returns �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipVia::ParseSentProtocol( const char * pszText, int iTextLen )
{
	int		iPos, iPrevPos = -1;
	char	cType = 0;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == '/' )
		{
			switch( cType )
			{
			case 0:
				m_strProtocolName.append( pszText, iPos );
				iPrevPos = iPos + 1;
				break;
			case 1:
				m_strProtocolVersion.append( pszText + iPrevPos, iPos - iPrevPos );
				iPrevPos = iPos + 1;
				break;
			default:
				return -1;
			}
			++cType;
		}
		else if( pszText[iPos] == ' ' )
		{
			if( cType == 2 )
			{
				m_strTransport.append( pszText + iPrevPos, iPos - iPrevPos );
				return iPos + 1;
			}
			else
			{
				return -1;
			}
		}
	}
	
	return -1;
}

/**
 * @brief ���� ȣ��Ʈ ������ �Ľ��Ѵ�.
 * @param pszText		���� ȣ��Ʈ ���� ���ڿ�
 * @param iTextLen	���� ȣ��Ʈ ���� ���ڿ� ����
 * @returns �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipVia::ParseSentBy( const char * pszText, int iTextLen )
{
	int		iPos, iPortPos = -1;

	if( pszText[0] == '[' )
	{
		bool bIpFound = false;

		for( iPos = 1; iPos < iTextLen; ++iPos )
		{
			if( bIpFound == false )
			{
				if( pszText[iPos] == ']' )
				{
					m_strHost.append( pszText + 1, iPos - 1 );
					bIpFound = true;
				}
			}
			else if( pszText[iPos] == ':' )
			{
				iPortPos = iPos + 1;
			}
			else if( pszText[iPos] == ' ' || pszText[iPos] == ';' || pszText[iPos] == ',' )
			{
				break;
			}
		}

		if( bIpFound == false ) return -1;
		if( iPortPos != -1 && iPortPos < iPos )
		{
			std::string strTemp;

			strTemp.append( pszText + iPortPos, iPos - iPortPos );
			m_iPort = atoi( strTemp.c_str() );
		}

		return iPos;
	}
	else
	{
		for( iPos = 0; iPos < iTextLen; ++iPos )
		{
			if( pszText[iPos] == ':' )
			{
				m_strHost.append( pszText, iPos );
				iPortPos = iPos + 1;
			}
			else if( pszText[iPos] == ' ' || pszText[iPos] == ';' || pszText[iPos] == ',' )
			{
				break;
			}
		}

		if( m_strHost.empty() )
		{
			m_strHost.append( pszText, iPos );
		}
		else
		{
			std::string	strPort;

			strPort.append( pszText + iPortPos, iPos - iPortPos );
			m_iPort = atoi( strPort.c_str() );
		}
	}

	return iPos;
}

/**
 * @ingroup SipParser
 * @brief SIP ��� ���ڿ��� �Ľ��Ͽ� CSipVia ��ü ����Ʈ�� �����Ѵ�.
 * @param clsList CSipVia ��ü ����Ʈ
 * @param pszText �Ľ��� ���ڿ�
 * @param iTextLen �Ľ��� ���ڿ��� ����
 * @returns �����ϸ� �Ľ��� ���ڿ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int ParseSipVia( SIP_VIA_LIST & clsList, const char * pszText, int iTextLen )
{
	int iPos, iCurPos = 0;
	CSipVia	clsVia;

	while( iCurPos < iTextLen )
	{
		if( pszText[iCurPos] == ' ' || pszText[iCurPos] == '\t' || pszText[iCurPos] == ',' )
		{
			++iCurPos;
			continue;
		}

		iPos = clsVia.Parse( pszText + iCurPos, iTextLen - iCurPos );
		if( iPos == -1 ) return -1;
		iCurPos += iPos;

		clsList.push_back( clsVia );
	}

	return iCurPos;
}

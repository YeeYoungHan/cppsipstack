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

#include "SipPlatformDefine.h"
#include "SdpMedia.h"
#include <stdlib.h>
#include "MemoryDebug.h"

/**
 * @ingroup SdpParser
 * @brief ������
 */
CSdpMedia::CSdpMedia() : m_iPort(-1), m_iNumOfPort(-1)
{
}

/**
 * @ingroup SdpParser
 * @brief ������
 * @param pszMedia		�̵�� �̸� 
 * @param iPort				��Ʈ ��ȣ
 * @param pszProtocol ���� ��������
 */
CSdpMedia::CSdpMedia( const char * pszMedia, int iPort, const char * pszProtocol ) : 
	m_strMedia(pszMedia), m_iPort(iPort), m_iNumOfPort(-1), m_strProtocol(pszProtocol)
{
}

/**
 * @ingroup SdpParser
 * @brief �Ҹ���
 */
CSdpMedia::~CSdpMedia()
{
}

/**
 * @ingroup SdpParser
 * @brief SDP media �� value ���ڿ��� �Ľ��Ѵ�.
 * @param pszText		SDP media �� value ���ڿ�
 * @param iTextLen	SDP media �� value ���ڿ� ����
 * @returns �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1�� �����Ѵ�.
 */
int CSdpMedia::Parse( const char * pszText, int iTextLen )
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

	if( iType < 3 ) return -1;

	return iTextLen;
}

/**
 * @ingroup SdpParser
 * @brief SDP media �� value ���ڿ��� �����Ѵ�.
 * @param pszText		SDP media �� value ���ڿ��� ������ ����
 * @param iTextSize pszText ������ ũ��
 * @returns �����ϸ� ����� ���ڿ� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSdpMedia::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	int iLen, n;
	SDP_FMT_LIST::iterator	itFmt;

	iLen = snprintf( pszText, iTextSize, "%s ", m_strMedia.c_str() );

	if( m_iNumOfPort == -1 )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "%d ", m_iPort );
	}
	else
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "%d/%d ", m_iPort, m_iNumOfPort );
	}

	iLen += snprintf( pszText + iLen, iTextSize - iLen, "%s", m_strProtocol.c_str() );

	for( itFmt = m_clsFmtList.begin(); itFmt != m_clsFmtList.end(); ++itFmt )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, " %s", itFmt->c_str() );
	}

	iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );

	if( m_strTitle.empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "i=%s\r\n", m_strTitle.c_str() );
	}

	if( m_clsConnection.Empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "c=" );
		n = m_clsConnection.ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	if( m_clsBandWidthList.empty() == false )
	{
		SDP_BANDWIDTH_LIST::iterator	itBL;

		for( itBL = m_clsBandWidthList.begin(); itBL != m_clsBandWidthList.end(); ++itBL )
		{
			iLen += snprintf( pszText + iLen, iTextSize - iLen, "b=" );
			n = itBL->ToString( pszText + iLen, iTextSize - iLen );
			if( n == -1 ) return -1;
			iLen += n;
			iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
		}
	}

	if( m_clsAttributeList.empty() == false )
	{
		SDP_ATTRIBUTE_LIST::iterator	itAL;

		for( itAL = m_clsAttributeList.begin(); itAL != m_clsAttributeList.end(); ++itAL )
		{
			iLen += snprintf( pszText + iLen, iTextSize - iLen, "a=" );
			n = itAL->ToString( pszText + iLen, iTextSize - iLen );
			if( n == -1 ) return -1;
			iLen += n;
			iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
		}
	}

	return iLen;
}

/**
 * @ingroup SdpParser
 * @brief ��� ������ �ʱ�ȭ��Ų��.
 */
void CSdpMedia::Clear()
{
	m_strMedia.clear();
	m_iPort = -1;
	m_iNumOfPort = -1;
	m_strProtocol.clear();
	m_clsFmtList.clear();
	m_strTitle.clear();
	m_clsConnection.Clear();
	m_clsBandWidthList.clear();
	m_clsAttributeList.clear();
}

/**
 * @ingroup SdpParser
 * @brief FMT �� �߰��Ѵ�.
 * @param iPayLoadType payload type
 */
void CSdpMedia::AddFmt( int iPayLoadType )
{
	if( SelectFmt( iPayLoadType ) ) return;

	char	szNum[11];

	snprintf( szNum, sizeof(szNum), "%d", iPayLoadType );

	m_clsFmtList.push_back( szNum );
}

/**
 * @ingroup SdpParser
 * @brief FMT ����Ʈ�� payload type �� �����ϴ��� Ȯ���Ѵ�. 
 * @param iPayLoadType payload type
 * @returns FMT ����Ʈ�� payload type �� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSdpMedia::SelectFmt( int iPayLoadType )
{
	char	szNum[11];
	SDP_FMT_LIST::iterator	itList;

	snprintf( szNum, sizeof(szNum), "%d", iPayLoadType );

	for( itList = m_clsFmtList.begin(); itList != m_clsFmtList.end(); ++itList )
	{
		if( !strcmp( itList->c_str(), szNum ) )
		{
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SdpParser
 * @brief ��Ʈ����Ʈ�� �����Ѵ�.
 * @param pszName ������ ��Ʈ����Ʈ �̸�
 * @returns ��Ʈ����Ʈ�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSdpMedia::DeleteAttribute( const char * pszName )
{
	SDP_ATTRIBUTE_LIST::iterator	itAttr, itNext;
	bool bRes = false;

	for( itAttr = m_clsAttributeList.begin(); itAttr != m_clsAttributeList.end(); ++itAttr )
	{
LOOP_START:
		if( !strcmp( itAttr->m_strName.c_str(), pszName ) )
		{
			bRes = true;

			itNext = itAttr;
			++itNext;

			m_clsAttributeList.erase( itAttr );

			if( itNext == m_clsAttributeList.end() ) break;
			itAttr = itNext;
			goto LOOP_START;
		}
	}

	return bRes;
}

/**
 * @ingroup SdpParser
 * @brief FMT ����Ʈ�� payload type �� �����ϸ� �ش� payload type �� FMT ����Ʈ �� ��Ʈ����Ʈ���� �����Ѵ�.
 * @param iPayLoadType payload type
 * @returns FMT ����Ʈ�� payload type �� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSdpMedia::DeleteFmtAttribute( int iPayLoadType )
{
	SDP_ATTRIBUTE_LIST::iterator	itAttr, itNext;
	SDP_FMT_LIST::iterator itFL;
	bool bRes = false;

	for( itFL = m_clsFmtList.begin(); itFL != m_clsFmtList.end(); ++itFL )
	{
		int iCodec = atoi( itFL->c_str() );

		if( iCodec == iPayLoadType )
		{
			m_clsFmtList.erase( itFL );
			bRes = true;
			break;
		}
	}

	if( bRes )
	{
		for( itAttr = m_clsAttributeList.begin(); itAttr != m_clsAttributeList.end(); ++itAttr )
		{
LOOP_START:
			if( !strcmp( itAttr->m_strName.c_str(), "rtpmap" ) || !strcmp( itAttr->m_strName.c_str(), "fmtp" ) )
			{
				int iCodec = atoi( itAttr->m_strValue.c_str() );

				if( iCodec == iPayLoadType )
				{
					itNext = itAttr;
					++itNext;

					m_clsAttributeList.erase( itAttr );
					
					if( itNext == m_clsAttributeList.end() ) break;
					itAttr = itNext;
					goto LOOP_START;
				}
			}
		}
	}

	return bRes;
}

/**
 * @ingroup SdpParser
 * @brief FMT ����Ʈ�� payload type �� �����ϸ� �ش� payload type �� FMT ����Ʈ �� ��Ʈ����Ʈ�� �ֻ������ �̵��Ѵ�.
 * @param iPayLoadType payload type
 * @returns FMT ����Ʈ�� payload type �� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSdpMedia::MakeTopFmtAttribute( int iPayLoadType )
{
	SDP_ATTRIBUTE_LIST::iterator	itAttr, itBegin;
	SDP_FMT_LIST::iterator itFL;
	bool bRes = false;

	for( itFL = m_clsFmtList.begin(); itFL != m_clsFmtList.end(); ++itFL )
	{
		int iCodec = atoi( itFL->c_str() );

		if( iCodec == iPayLoadType )
		{
			if( itFL == m_clsFmtList.begin() )
			{
				// �Է��� payload type �� top �̸� ������ �ʿ䰡 ����.
				return true;
			}

			std::string strFmt = *itFL;
			m_clsFmtList.erase( itFL );
			m_clsFmtList.push_front( strFmt );
			bRes = true;
			break;
		}
	}

	if( bRes )
	{
		itBegin = m_clsAttributeList.end();

		for( itAttr = m_clsAttributeList.begin(); itAttr != m_clsAttributeList.end(); ++itAttr )
		{
			if( itBegin == m_clsAttributeList.end() && 
					( !strcmp( itAttr->m_strName.c_str(), "rtpmap" ) || !strcmp( itAttr->m_strName.c_str(), "fmtp" ) ) )
			{
				itBegin = itAttr;
			}

			if( !strcmp( itAttr->m_strName.c_str(), "fmtp" ) )
			{
				int iCodec = atoi( itAttr->m_strValue.c_str() );

				if( iCodec == iPayLoadType )
				{
					CSdpAttribute clsAttr = *itAttr;
					m_clsAttributeList.erase( itAttr );
					m_clsAttributeList.insert( itBegin, clsAttr );
					break;
				}
			}
		}

		itBegin = m_clsAttributeList.end();

		for( itAttr = m_clsAttributeList.begin(); itAttr != m_clsAttributeList.end(); ++itAttr )
		{
			if( itBegin == m_clsAttributeList.end() && 
					( !strcmp( itAttr->m_strName.c_str(), "rtpmap" ) || !strcmp( itAttr->m_strName.c_str(), "fmtp" ) ) )
			{
				itBegin = itAttr;
			}

			if( !strcmp( itAttr->m_strName.c_str(), "rtpmap" ) )
			{
				int iCodec = atoi( itAttr->m_strValue.c_str() );

				if( iCodec == iPayLoadType )
				{
					CSdpAttribute clsAttr = *itAttr;
					m_clsAttributeList.erase( itAttr );
					m_clsAttributeList.insert( itBegin, clsAttr );
					break;
				}
			}
		}
	}

	return bRes;
}

/**
 * @ingroup SdpParser
 * @brief ��Ʈ����Ʈ�� �߰��Ѵ�. rtpmap �̸� fmt �� �߰��Ѵ�.
 * @param pclsAttr ��Ʈ����Ʈ ��ü
 */
void CSdpMedia::AddAttribute( CSdpAttribute * pclsAttr )
{
	if( !strcmp( pclsAttr->m_strName.c_str(), "rtpmap" ) )
	{
		AddFmt( pclsAttr->GetPayLoadType() );
	}

	m_clsAttributeList.push_back( *pclsAttr );
}

/**
 * @ingroup SdpParser
 * @brief ��Ʈ����Ʈ�� �߰��Ѵ�. rtpmap �̸� fmt �� �߰��Ѵ�.
 * @param pszName		��Ʈ����Ʈ �̸�
 * @param pszValue	��Ʈ����Ʈ ��
 */
void CSdpMedia::AddAttribute( const char * pszName, const char * pszValue )
{
	CSdpAttribute clsAttr( pszName, pszValue );

	if( !strcmp( clsAttr.m_strName.c_str(), "rtpmap" ) )
	{
		AddFmt( clsAttr.GetPayLoadType() );
	}

	m_clsAttributeList.push_back( clsAttr );
}

/**
 * @ingroup SdpParser
 * @brief direction ��Ʈ���並 �����Ѵ�.
 * @param pszDirection direction
 */
void CSdpMedia::SetDirection( const char * pszDirection )
{
	SDP_ATTRIBUTE_LIST::iterator	itAttr;
	bool bFound = false;

	for( itAttr = m_clsAttributeList.begin(); itAttr != m_clsAttributeList.end(); ++itAttr )
	{
		if( !strcmp( itAttr->m_strName.c_str(), "sendrecv" ) ||
				!strcmp( itAttr->m_strName.c_str(), "sendonly" ) ||
				!strcmp( itAttr->m_strName.c_str(), "recvonly" ) ||
				!strcmp( itAttr->m_strName.c_str(), "inactive" ) )
		{
			if( strcmp( itAttr->m_strName.c_str(), pszDirection ) )
			{
				itAttr->m_strName = pszDirection;
			}

			bFound = true;
			break;
		}
	}

	if( bFound == false )
	{
		AddAttribute( pszDirection, NULL );
	}
}

/**
 * @ingroup SdpParser
 * @brief SDP media ���ڿ��� �Ľ��� �� �׸��� �����Ѵ�.
 * @param pszData ���ڿ�
 * @param iLen		���ڿ� ����
 * @param iType		SDP media ���ڿ��� �Ľ��� ���ڿ� �迭�� �ε���
 */
void CSdpMedia::SetData( const char * pszData, int iLen, int iType )
{
	switch( iType )
	{
	case 0:
		m_strMedia.append( pszData, iLen );
		break;
	case 1:
		for( int i = 0; i < iLen; ++i )
		{
			if( pszData[i] == '/' )
			{
				std::string	strTemp;

				strTemp.append( pszData, i );
				m_iPort = atoi( strTemp.c_str() );

				strTemp.clear();
				strTemp.append( pszData + i + 1, iLen - ( i + 1 ) );
				break;
			}
		}

		if( m_iPort == -1 )
		{
			std::string	strTemp;

			strTemp.append( pszData, iLen );
			m_iPort = atoi( strTemp.c_str() );
		}
		break;
	case 2:
		m_strProtocol.append( pszData, iLen );
		break;
	default:
		{
			std::string strTemp;

			strTemp.append( pszData, iLen );
			m_clsFmtList.push_back( strTemp );
		}
		break;
	}
}

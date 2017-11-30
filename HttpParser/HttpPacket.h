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

#ifndef _HTTP_PACKET_H_
#define _HTTP_PACKET_H_

#include "HttpMessage.h"

/**
 * @ingroup HttpParser
 * @brief HTTP 프로토콜 파싱 단계
 */
enum EHttpPacketStatus
{
	H_HPS_HEADER = 0,
	H_HPS_BODY,
	H_HPS_BODY_END
};

/**
 * @ingroup HttpParser
 * @brief 수신된 HTTP 프로토콜 패킷을 저장하고 파싱하는 클래스
 */
class CHttpPacket
{
public:
	CHttpPacket();
	~CHttpPacket();

	bool AddPacket( const char * pszPacket, int iPacketLen );
	bool IsCompleted( );
	void ClearMessage( );

	CHttpMessage * GetHttpMessage( );

private:
	CHttpMessage m_clsMessage;
	std::string m_strBuf;
	int m_iChunkedLen;
	EHttpPacketStatus m_eStatus;

	bool ParseChunkedHeader( );
	bool ParseChunkedBody( );
};

#endif

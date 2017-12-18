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

#ifndef _SIP_CALL_DUMP_SETUP_H_
#define _SIP_CALL_DUMP_SETUP_H_

#define DEFAULT_PACKET_SNAP_LEN					2048
#define DEFAULT_PACKET_READ_TIMEOUT			200

#include "XmlElement.h"

class CSipCallDumpSetup
{
public:
	CSipCallDumpSetup();
	~CSipCallDumpSetup();

	bool Read( const char * pszFileName );
	bool Read( );

	bool IsChange();

	// 패킷 덤프 설정
	int		m_iPacketSnapLen;								// pcap_open 의 2번째 인자값
	int		m_iPacketReadTimeout;						// pcap_open 의 4번째 인자값

private:
	bool Read( CXmlElement & clsXml );
	void SetFileSizeTime( );

	std::string	m_strFileName;	// 설정 파일 이름
	time_t			m_iFileTime;		// 설정 파일 저장 시간
	int					m_iFileSize;		// 설정 파일 크기
};

extern CSipCallDumpSetup gclsSetup;

#endif

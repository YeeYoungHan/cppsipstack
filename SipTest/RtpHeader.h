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

#ifndef _RTP_HEADER_H_
#define _RTP_HEADER_H_

/**
 * @ingroup SipTest
 * @brief RTP 헤더를 저장하는 클래스
 */
class CRtpHeader
{
public:
  unsigned char GetVersion() 
	{
		return m_cFlags >> 6; 
	}

  void SetVersion( unsigned char nVersion )
  {    
		m_cFlags = (nVersion << 6) | (m_cFlags & 0x3F);
  }

  unsigned char GetPadding() 
	{ 
		return (m_cFlags >> 5) & 0x1; 
	}

  void SetPadding( unsigned char bPadding ) 
  { 
    m_cFlags = (bPadding << 5) | (m_cFlags & 0xDF);
  }

  unsigned char GetExtension() 
	{ 
		return (m_cFlags >> 4) & 0x01;
	}
    
	void SetExtension(unsigned char bExt)
  {
    m_cFlags = (bExt << 4) | (m_cFlags & 0xEF);
  }

  unsigned char GetCC() 
	{ 
		return m_cFlags & 0x0F;
	}
    
	void SetCC(unsigned char nCC)
  {
    m_cFlags = nCC | (m_cFlags & 0xF0);
  }

  unsigned char GetMarker() 
	{ 
		return m_cMpt >> 7; 
	}

  void SetMarker(unsigned char bMarker)
  {
    m_cMpt = (bMarker << 7) | (m_cMpt & 0x7F);
  }

  unsigned char GetPT() 
	{ 
		return m_cMpt & 0x7F;
	}
    
	void SetPT(unsigned char nPT)
  {
    m_cMpt = nPT | (m_cMpt & 0x80);
  }

	void SetSeq( unsigned short sSeq )
	{
		m_sSeq = htons( sSeq );
	}

	unsigned short GetSeq()
	{
		return ntohs( m_sSeq );
	}

	void SetTimeStamp( unsigned int iTimeStamp )
	{
		m_iTimeStamp = htonl( iTimeStamp );
	}

	unsigned int GetTimeStamp( )
	{
		return ntohl( m_iTimeStamp );
	}

	void SetSsrc( unsigned int iSsrc )
	{
		m_iSsrc = htonl( iSsrc );
	}

	unsigned int GetSsrc( )
	{
		return ntohl( m_iSsrc );
	}

private:
  unsigned char  m_cFlags;
  unsigned char  m_cMpt;
  unsigned short m_sSeq;         // sequence number
  unsigned int	 m_iTimeStamp;   // timestamp
  unsigned int   m_iSsrc;        // synchronization source
};

#endif

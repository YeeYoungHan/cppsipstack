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

#include "SipPlatformDefine.h"
#include "SipUdp.h"

/**
 * @ingroup RtpApi
 * @brief RTP ����� �����ϴ� ����ü
 */
typedef struct _RtpHeader_
{
  uint8_t		cFlags;				// version + padding + extension
  uint8_t		cMpt;					// marker + payload type
  uint16_t	sSeq;         // sequence number
  uint32_t	iTimeStamp;   // timestamp
  uint32_t  ssrc;					// synchronization source

  /**
	 * @ingroup SipClientMFC
	 * @brief RTP ���� ������ �����´�.
	 * @returns RTP ���� ������ �����Ѵ�.
	 */
	inline uint8_t GetVersion() 
	{
		return cFlags >> 6; 
	}

  /**
	 * @ingroup SipClientMFC
	 * @brief RTP ���� ������ �����Ѵ�.
	 * @param cVersion RTP ���� ����
	 */
	inline void SetVersion( uint8_t cVersion )
  {    
		cFlags = (cVersion << 6) | (cFlags & 0x3F);
  }

  /**
	 * @ingroup SipClientMFC
	 * @brief Padding �� �����Ѵ�.
	 * @returns Padding �� �����Ѵ�.
	 */
	inline uint8_t GetPadding() 
	{ 
		return (cFlags >> 5) & 0x1; 
	}

  /**
	 * @ingroup SipClientMFC
	 * @brief Padding �� �����Ѵ�.
	 * @param cPadding Padding
	 */
	inline void SetPadding( uint8_t cPadding ) 
  { 
		cFlags = (cPadding << 5) | (cFlags & 0xDF);
  }

  /**
	 * @ingroup SipClientMFC
	 * @brief Extension �� �����Ѵ�.
	 * @returns Extension �� �����Ѵ�.
	 */
	inline uint8_t GetExtension() 
	{ 
		return (cFlags >> 4) & 0x01;
	}
    
	/**
	 * @ingroup SipClientMFC
	 * @brief Extension �� �����Ѵ�.
	 * @param cExt Extension
	 */
	inline void SetExtension( uint8_t cExt )
  {
		cFlags = (cExt << 4) | (cFlags & 0xEF);
  }

  /**
	 * @ingroup SipClientMFC
	 * @brief CSRC count �� �����Ѵ�.
	 * @returns CSRC count �� �����Ѵ�.
	 */
	inline uint8_t GetCC() 
	{ 
		return cFlags & 0x0F;
	}
    
	/**
	 * @ingroup SipClientMFC
	 * @brief CSRC count �� �����Ѵ�.
	 * @param cCC CSRC count
	 */
	inline void SetCC( uint8_t cCC )
  {
		cFlags = cCC | (cFlags & 0xF0);
  }

  /**
	 * @ingroup SipClientMFC
	 * @brief Marker �� �����Ѵ�.
	 * @returns Marker �� �����Ѵ�.
	 */
	inline uint8_t GetMarker() 
	{ 
		return cMpt >> 7; 
	}

  /**
	 * @ingroup SipClientMFC
	 * @brief Marker �� �����Ѵ�.
	 * @param cMarker Marker
	 */
	inline void SetMarker( uint8_t cMarker )
  {
		cMpt = (cMarker << 7) | (cMpt & 0x7F);
  }

  /**
	 * @ingroup SipClientMFC
	 * @brief Payload type �� �����Ѵ�.
	 * @returns Payload type �� �����Ѵ�.
	 */
	inline uint8_t GetPT() 
	{ 
		return cMpt & 0x7F;
	}
    
	/**
	 * @ingroup SipClientMFC
	 * @brief Payload type �� �����Ѵ�.
	 * @param cPT Payload type
	 */
	inline void SetPT( uint8_t cPT )
  {
		cMpt = cPT | (cMpt & 0x80);
  }

	/**
	 * @ingroup SipClientMFC
	 * @brief Sequence number �� �����Ѵ�.
	 * @returns Sequence number �� �����Ѵ�.
	 */
	inline uint16_t GetSeq( )
	{
		return ntohs(sSeq);
	}

	/**
	 * @ingroup SipClientMFC
	 * @brief Sequence number �� �����Ѵ�.
	 * @param sSeqIn Sequence number
	 */
	void SetSeq( uint16_t sSeqIn )
	{
		sSeq = htons(sSeqIn);
	}

	/**
	 * @ingroup SipClientMFC
	 * @brief Timestamp �� �����Ѵ�.
	 * @returns Timestamp �� �����Ѵ�.
	 */
	inline uint32_t GetTimeStamp( )
	{
		return ntohl(iTimeStamp);
	}

	/**
	 * @ingroup SipClientMFC
	 * @brief Timestamp �� �����Ѵ�.
	 * @param iTime Timestamp
	 */
	void SetTimeStamp( uint32_t iTime )
	{
		iTimeStamp = htonl( iTime );
	}

} RtpHeader;


#endif

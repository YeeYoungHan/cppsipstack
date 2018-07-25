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

#ifndef _H264_DECODER_H_
#define _H264_DECODER_H_

#include "SipPlatformDefine.h"
#include "codec_api.h"

class CH264Decoder
{
public:
	CH264Decoder();
	~CH264Decoder();

	bool Create( const char * pszDllName );
	bool Destory( );

	bool Decode( uint8_t * pInput, int iInputLen );

private:
	ISVCDecoder * m_pSvcDecoder;
	bool	m_bInit;

	uint8_t		* m_pRGB;
	int				m_iWidth;
	int				m_iHeight;
};

#endif

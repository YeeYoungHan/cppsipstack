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

#include "H264Decoder.h"
#include <stdio.h>
#include <windows.h>

typedef int( __cdecl * H264CreateDecoder )( ISVCDecoder ** ppDecoder );
typedef void( __cdecl * H264DestroyDecoder )( ISVCDecoder * ppDecoder );

static H264CreateDecoder CreateDecoder;
static H264DestroyDecoder DestroyDecoder;

CH264Decoder::CH264Decoder() : m_pSvcDecoder(NULL), m_bInit(false), m_pRGB(NULL), m_iWidth(0), m_iHeight(0)
{
}

CH264Decoder::~CH264Decoder()
{
	Destory( );
}

bool CH264Decoder::Create( const char * pszDllName )
{
	if( CreateDecoder == NULL || DestroyDecoder == NULL )
	{
		HMODULE hDll = LoadLibrary( pszDllName );
		if( hDll == NULL )
		{
			return false;
		}

		CreateDecoder = (H264CreateDecoder)GetProcAddress( hDll, "WelsCreateDecoder" );
		if( CreateDecoder == NULL ) return false;

		DestroyDecoder = (H264DestroyDecoder)GetProcAddress( hDll, "WelsDestroyDecoder" );
		if( DestroyDecoder == NULL ) return false;
	}

	if( CreateDecoder( &m_pSvcDecoder ) != 0 )
	{
		return false;
	}

	SDecodingParam sttParam;

	memset( &sttParam, 0, sizeof(sttParam) );
	sttParam.uiTargetDqLayer = UCHAR_MAX;
	sttParam.eEcActiveIdc = ERROR_CON_SLICE_COPY;
	sttParam.sVideoProperty.eVideoBsType = VIDEO_BITSTREAM_DEFAULT;

	if( m_pSvcDecoder->Initialize( &sttParam ) != 0 )
	{
		return false;
	}

	m_bInit = true;

	return true;
}

bool CH264Decoder::Destory( )
{
	if( m_pSvcDecoder )
	{
		if( m_bInit )
		{
			m_pSvcDecoder->Uninitialize();
			m_bInit = false;
		}

		DestroyDecoder( m_pSvcDecoder );
		m_pSvcDecoder = NULL;
	}

	if( m_pRGB )
	{
		free( m_pRGB );
		m_pRGB = NULL;
	}

	return true;
}

static inline uint8_t GetUint8( int iValue )
{
	if( iValue < 0 )
	{
		return 0;
	}
	else if( iValue > 255 )
	{
		return 255;
	}

	return (uint8_t)iValue;
}

// https://en.wikipedia.org/wiki/YUV
static inline void SaveRGB( int iC, int iD, int iE, uint8_t * pRGB )
{
	pRGB[0] = GetUint8( (298 * iC + 409 * iE + 128) >> 8 );
	pRGB[1] = GetUint8( (298 * iC - 100 * iD - 208 * iE + 128) >> 8 );
	pRGB[2] = GetUint8( (298 * iC + 516 * iD + 128) >> 8 );
}

bool CH264Decoder::Decode( uint8_t * pInput, int iInputLen )
{
	uint8_t * arrBuf[3];	// 0:Y, 1:U, 2:V
	SBufferInfo sttBufInfo;
	
	memset( &sttBufInfo, 0, sizeof(sttBufInfo));
	if( m_pSvcDecoder->DecodeFrame2( pInput, iInputLen, arrBuf, &sttBufInfo ) != 0 )
	{
		return false;
	}

	if( sttBufInfo.iBufferStatus != 1 )
	{
		return false;
	}

	int iWidth = sttBufInfo.UsrData.sSystemBuffer.iWidth;
	int iHeight = sttBufInfo.UsrData.sSystemBuffer.iHeight;
	int iStribe = sttBufInfo.UsrData.sSystemBuffer.iStride[0];

	uint8_t * pY = arrBuf[0];
	uint8_t * pU = arrBuf[1];
	uint8_t * pV = arrBuf[2];

	// YUV422 -> RGB
	uint8_t	* pRGB;
	int aC[2], iD, iE;

	if( m_pRGB == NULL || m_iWidth != iWidth || m_iHeight != iHeight )
	{
		if( m_pRGB ) free( m_pRGB );

		m_pRGB = (uint8_t *)malloc( 3 * iWidth * iHeight );
		if( m_pRGB == NULL )
		{
			return false;
		}
	}

	pRGB = m_pRGB;
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	for( int i = 0; i < iHeight; ++i )
	{
		for( int iW = 0; iW < iWidth; iW += 2 )
		{
			aC[0] = pY[0] - 16;
			aC[1] = pY[1] - 16;
			iD = pU[0] - 128;
			iE = pV[0] - 128;

			SaveRGB( aC[0], iD, iE, pRGB );
			pRGB += 3;

			SaveRGB( aC[1], iD, iE, pRGB );
			pRGB += 3;

			pY += 2;
			++pU;
			++pV;
		}
	}

	return true;
}

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

#include "stdafx.h"
#include "WinAudio.h"
#include "Log.h"
#include "MemoryDebug.h"

#pragma comment( lib, "Winmm" )

#define SAMPLE_RATE	8000

CWinAudio::CWinAudio() : m_pclsCallBack(NULL), m_hWaveIn(NULL), m_hWaveOut(NULL)
{
	memset( m_arrPcmIn, 0, sizeof(m_arrPcmIn) );
	memset( m_arrPcmOut, 0, sizeof(m_arrPcmOut) );
}

CWinAudio::~CWinAudio()
{
}

static void CALLBACK waveInProc( HWAVEIN hWaveIn, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2 )
{
	switch(uMsg)
	{
	case WIM_DATA:
		{
			CWinAudio * pclsAudio = (CWinAudio *)dwInstance;
			WAVEHDR * psttWaveHdr = (WAVEHDR *)dwParam1;

			if( pclsAudio && pclsAudio->m_pclsCallBack )
			{
				pclsAudio->m_pclsCallBack->EventInPcm( (int16_t *)psttWaveHdr->lpData, 160 );
			}

			waveInPrepareHeader( hWaveIn, psttWaveHdr, sizeof(WAVEHDR) );
			waveInAddBuffer( hWaveIn, psttWaveHdr, sizeof(WAVEHDR) );
		}
		break;
	}
}

/**
 * @ingroup SipClientMFC
 * @brief 오디오 입력/출력을 시작한다.
 * @param pclsCallBack 오디오 입력 callback 객체의 포인터
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CWinAudio::Open( IWinAudioCallBack * pclsCallBack )
{
	if( m_hWaveOut && m_hWaveIn ) return true;

	WAVEFORMATEX sttFormat;
	MMRESULT res;
 
	sttFormat.wFormatTag = WAVE_FORMAT_PCM;
	sttFormat.nChannels = 1;
	sttFormat.nSamplesPerSec = SAMPLE_RATE;
	sttFormat.nAvgBytesPerSec = 2 * SAMPLE_RATE;
	sttFormat.nBlockAlign = 2;
	sttFormat.wBitsPerSample = 16;
	sttFormat.cbSize = 0;

	res = waveInOpen( &m_hWaveIn, WAVE_MAPPER, &sttFormat, (DWORD_PTR)waveInProc, (DWORD_PTR)this, CALLBACK_FUNCTION );
	if( res != MMSYSERR_NOERROR )
	{
		CLog::Print( LOG_ERROR, "waveInOpen error" );
		return false;
	}

	res = waveOutOpen( &m_hWaveOut, WAVE_MAPPER, &sttFormat, 0, 0, WAVE_FORMAT_DIRECT );
	if( res != MMSYSERR_NOERROR )
	{
		CLog::Print( LOG_ERROR, "waveOutOpen error" );
		Close();
		return false;
	}

	for( int i = 0; i < PCM_OUT_BUF_COUNT; ++i )
	{
		m_arrWaveHdrOut[i].lpData = (LPSTR)m_arrPcmOut[i];
		m_arrWaveHdrOut[i].dwBufferLength = 320;
		m_arrWaveHdrOut[i].dwBytesRecorded = 320;
		m_arrWaveHdrOut[i].dwUser = 0;
		m_arrWaveHdrOut[i].dwFlags = WHDR_DONE;
		m_arrWaveHdrOut[i].dwLoops = 0;
	}

	m_sttWaveHdrIn.lpData = (LPSTR)m_arrPcmIn;
	m_sttWaveHdrIn.dwBufferLength = 320;
	m_sttWaveHdrIn.dwBytesRecorded = 0;
	m_sttWaveHdrIn.dwUser = 0;
	m_sttWaveHdrIn.dwFlags = 0;
	m_sttWaveHdrIn.dwLoops = 0;

	waveInPrepareHeader( m_hWaveIn, &m_sttWaveHdrIn, sizeof(m_sttWaveHdrIn) );
	res = waveInAddBuffer( m_hWaveIn, &m_sttWaveHdrIn, sizeof(m_sttWaveHdrIn) );
	if( res != MMSYSERR_NOERROR )
	{
		CLog::Print( LOG_ERROR, "waveInAddBuffer error" );
		Close();
		return false;
	}

	res = waveInStart( m_hWaveIn );
	if( res != MMSYSERR_NOERROR )
	{
		CLog::Print( LOG_ERROR, "waveInStart error" );
		Close();
		return false;
	}

	m_pclsCallBack = pclsCallBack;

	return true;
}

/**
 * @ingroup SipClientMFC
 * @brief 오디오 입력/출력을 중지한다.
 * @returns true 리턴한다.
 */
bool CWinAudio::Close( )
{
	if( m_hWaveOut )
	{
		waveOutClose( m_hWaveOut );
		m_hWaveOut = NULL;
	}

	if( m_hWaveIn )
	{
		waveInStop( m_hWaveIn );
		waveInClose( m_hWaveIn );
		m_hWaveIn = NULL;
	}

	return true;
}

/**
 * @ingroup SipClientMFC
 * @brief 오디오를 출력한다.
 * @param parrPcm	PCM 버퍼
 * @param iPcmLen PCM 버퍼 크기
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CWinAudio::OutPcm( const int16_t * parrPcm, int iPcmLen )
{
	bool bFound = false;

	for( int i = 0; i < PCM_OUT_BUF_COUNT; ++i )
	{
		if( m_arrWaveHdrOut[i].dwFlags & WHDR_DONE )
		{
			memcpy( m_arrPcmOut[i], parrPcm, iPcmLen * 2 );
			waveOutPrepareHeader( m_hWaveOut, &m_arrWaveHdrOut[i], sizeof(m_arrWaveHdrOut[i]) );
			waveOutWrite( m_hWaveOut, &m_arrWaveHdrOut[i], sizeof(m_arrWaveHdrOut[i]) );
			bFound = true;
			break;
		}
	}

	if( bFound == false )
	{
		printf( "no done\n" );
	}

	return true;
}

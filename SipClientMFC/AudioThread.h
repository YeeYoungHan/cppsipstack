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

#ifndef _AUDIO_THREAD_H_
#define _AUDIO_THREAD_H_

#include "WinAudio.h"
#include "SipUdp.h"

class CAudioThread : public IWinAudioCallBack
{
public:
	CAudioThread();
	virtual ~CAudioThread();

	bool Start();
	bool Stop();

	void SetDestIpPort( const char * pszDestIp, int iDestPort );

	virtual void EventInPcm( const int16_t * parrPcm, int iPcmLen );

	int			m_iPort;
	Socket	m_hSocket;

	std::string	m_strDestIp;
	int					m_iDestPort;

	uint16_t		m_sSeq;
	uint32_t		m_iTimeStamp;

	bool	m_bStop;
	bool	m_bThreadRun;

	CWinAudio	m_clsAudio;
};

#endif

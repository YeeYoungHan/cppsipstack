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

THREAD_API RtpThreadRecv( LPVOID lpParameter )
{
	pollfd sttPoll[1];
	char	szPacket[320], szPCM[320], szIp[21];
	int		iPacketLen;
	unsigned short sPort;

	gclsRtpThread.m_bRecvThreadRun = true;

	TcpSetPollIn( sttPoll[0], gclsRtpThread.m_hSocket );

#ifndef WIN32
	int n;
  snd_pcm_t * psttSound = NULL;
  snd_pcm_hw_params_t * psttParam;
  unsigned int iValue;

	n = snd_pcm_open( &psttSound, "default", SND_PCM_STREAM_PLAYBACK, 0 );
  if( CheckError( n, "snd_pcm_open" ) ) goto FUNC_END;

  snd_pcm_hw_params_alloca( &psttParam );

  snd_pcm_hw_params_any( psttSound, psttParam );

  n = snd_pcm_hw_params_set_access( psttSound, psttParam, SND_PCM_ACCESS_RW_INTERLEAVED );
  if( CheckError( n, "snd_pcm_hw_params_set_access" ) ) goto FUNC_END;

  n = snd_pcm_hw_params_set_format( psttSound, psttParam, SND_PCM_FORMAT_S16_LE );
  if( CheckError( n, "snd_pcm_hw_params_set_access" ) ) goto FUNC_END;

  n = snd_pcm_hw_params_set_channels( psttSound, psttParam, 1 );
  if( CheckError( n, "snd_pcm_hw_params_set_channels" ) ) goto FUNC_END;

  iValue = 8000;
  n = snd_pcm_hw_params_set_rate_near( psttSound, psttParam, &iValue, 0 );
  if( CheckError( n, "snd_pcm_hw_params_set_rate_near" ) ) goto FUNC_END;

  n = snd_pcm_hw_params( psttSound, psttParam );
  if( CheckError( n, "snd_pcm_hw_params" ) ) goto FUNC_END;
#endif

	while( gclsRtpThread.m_bStopEvent == false )
	{
		if( poll( sttPoll, 1, 200 ) <= 0 )
		{
			continue;
		}

		iPacketLen = sizeof(szPacket);
		if( UdpRecv( gclsRtpThread.m_hSocket, szPacket, &iPacketLen, szIp, sizeof(szIp), &sPort ) == false )
		{
			continue;
		}

		if( iPacketLen == 160 + sizeof(RtpHeader) )
		{
			UlawToPcm( szPacket + sizeof(RtpHeader), 160, szPCM, sizeof(szPCM) );

#ifndef WIN32
			n = snd_pcm_writei( psttSound, szPCM, sizeof(szPCM) / 2 );
			if( n == -EPIPE )
			{
				snd_pcm_prepare( psttSound );
			}
			else if( CheckError( n, "snd_pcm_writei" ) ) break;
#endif
		}
	}

#ifndef WIN32
FUNC_END:
	if( psttSound )
	{
		snd_pcm_drain( psttSound );
		snd_pcm_close( psttSound );
	}
#endif
	
	gclsRtpThread.m_bRecvThreadRun = false;

	return 0;
}

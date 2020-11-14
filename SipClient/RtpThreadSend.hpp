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

bool CheckError( int n, const char * pszLog )
{
	if( n < 0 )
	{
#ifndef WIN32
		printf( "%s error - %s\n", pszLog, snd_strerror(n));
#endif
    return true;
	}

	return false;
}

THREAD_API RtpThreadSend( LPVOID lpParameter )
{
  char szPacket[320], szRead[320];
	RtpHeader * psttRtpHeader = (RtpHeader *)szPacket;
	uint16_t		sSeq = 0;
	uint32_t		iTimeStamp = 0;

	gclsRtpThread.m_bSendThreadRun = true;

	psttRtpHeader->SetVersion(2);
	psttRtpHeader->SetPadding(0);
	psttRtpHeader->SetExtension(0);
	psttRtpHeader->SetCC(0);
	psttRtpHeader->SetMarker(0);
	psttRtpHeader->ssrc = htonl( 200 );

	psttRtpHeader->SetPT(0);

#ifndef WIN32
	int n;
  snd_pcm_t * psttSound = NULL;
  snd_pcm_hw_params_t * psttParam;
  unsigned int iValue;

	n = snd_pcm_open( &psttSound, gclsSetupFile.m_strMic.c_str(), SND_PCM_STREAM_CAPTURE, 0 );
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
#ifndef WIN32
		n = snd_pcm_readi( psttSound, szRead, sizeof(szRead) / 2 );
    if( CheckError( n, "snd_pcm_readi" ) ) break;
#endif
    
		psttRtpHeader->SetSeq( sSeq );
		psttRtpHeader->SetTimeStamp( iTimeStamp );

		++sSeq;
		iTimeStamp += 160;

		PcmToUlaw( szRead, 320, szPacket + sizeof(RtpHeader), 160 );

		UdpSend( gclsRtpThread.m_hSocket, szPacket, 160 + sizeof(RtpHeader), gclsRtpThread.m_strDestIp.c_str(), gclsRtpThread.m_iDestPort );
  }

#ifndef WIN32
FUNC_END:
	if( psttSound )
	{
		snd_pcm_drain( psttSound );
		snd_pcm_close( psttSound );
	}
#endif

	gclsRtpThread.m_bSendThreadRun = false;

	return 0;
}

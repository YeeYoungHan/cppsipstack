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

bool GetRtpPacket( pcap_t * psttPcap, char * pszRtpPacket, int * piRtpPacketLen )
{
	char		szIp[21];
	struct pcap_pkthdr * psttHeader;
	const		u_char * pszData;
	int			n, iIpHeaderLen, iIpPos, iPort, iPos;

	Ip4Header	* psttIp4Header;
	UdpHeader * psttUdpHeader;

	while( gclsRtpThread.m_bStopEvent == false )
	{
		n = pcap_next_ex( psttPcap, &psttHeader, &pszData );
		if( n <= 0 ) break;

		if( pszData[12] == 0x81 )
		{
			iIpPos = 18;		// VLAN
		}
		else if( pszData[12] == 0x08 )
		{
			iIpPos = 14;		// IP
		}
		else
		{
			continue;
		}

		psttIp4Header = (Ip4Header *)( pszData + iIpPos );

		// IPv4 만 검사한다.
		if( ( psttIp4Header->ver_ihl & 0xF0 ) != 0x40 ) continue;

		iIpHeaderLen = GetIpHeaderLength( psttIp4Header );
		if( iIpHeaderLen == 0 ) continue;

		inet_ntop( AF_INET, &psttIp4Header->daddr, szIp, sizeof(szIp) );
		if( strcmp( szIp, gclsSetupFile.m_strPcapRtpDestIp.c_str() ) ) continue;

		if( IsUdpPacket( psttIp4Header ) )
		{
			iPos = iIpPos + iIpHeaderLen;
			psttUdpHeader = (UdpHeader *)( pszData + iPos );
			
			iPort = ntohs( psttUdpHeader->dport );

			if( iPort == gclsSetupFile.m_iPcapRtpDestPort )
			{
				iPos += sizeof(UdpHeader);

				*piRtpPacketLen = psttHeader->caplen - iPos;
				memcpy( pszRtpPacket, pszData + iPos, *piRtpPacketLen );

				return true;
			}
		}
	}

	return false;
}

bool SrtpCreate( const char * pszAlgorithm, const char * pszKey, srtp_t * psttSrtp )
{
	srtp_policy_t sttPolicy;
	err_status_t	eError;
	char		szKey[100];
	int			iKeyLen = (int)strlen( pszKey );

	// RX
	memset( szKey, 0, sizeof(szKey) );
	if( Base64Decode( pszKey, (int)iKeyLen, szKey, sizeof(szKey) ) == -1 )
	{
		CLog::Print( LOG_ERROR, "%s Base64Decode error", __FUNCTION__ );
		return false;
	}
	
	if( !strcmp( pszAlgorithm, "AES_CM_128_HMAC_SHA1_80" ) )
	{
    crypto_policy_set_aes_cm_128_hmac_sha1_80( &sttPolicy.rtp );
  	crypto_policy_set_aes_cm_128_hmac_sha1_80( &sttPolicy.rtcp );
  }
	else if( !strcmp( pszAlgorithm, "AES_CM_128_HMAC_SHA1_32" ) )
	{
    crypto_policy_set_aes_cm_128_hmac_sha1_32( &sttPolicy.rtp );
  	crypto_policy_set_aes_cm_128_hmac_sha1_32( &sttPolicy.rtcp );
  }
	else
	{
		CLog::Print( LOG_ERROR, "%s error - algorithm(%s) is not defined", __FUNCTION__, pszAlgorithm );
	  return false;
	}
	
	sttPolicy.key = (uint8_t *)szKey;
	sttPolicy.ssrc.type = ssrc_any_inbound;
	sttPolicy.ssrc.value = 0;
	sttPolicy.next = NULL;

	eError = srtp_create( psttSrtp, &sttPolicy );
	if( eError != err_status_ok )
	{
		CLog::Print( LOG_ERROR, "%s srtp_create error(%d) - key(%s)", __FUNCTION__, eError, szKey );
		return false;
	}
  
	return true;
}

static bool gbSrtpInit = false;

THREAD_API RtpThreadSend( LPVOID lpParameter )
{
	char	szErrBuf[PCAP_ERRBUF_SIZE], szRtpPacket[1600];
	int		iMiliSecond, iRtpPacketLen, iRtpPayloadLen, iSleepMiliSecond = 20;

	struct timeval	sttTime, sttSendTime;
	srtp_t psttSrtp = NULL;
	err_status_t iRet;

	gettimeofday( &sttSendTime, NULL );

	gclsRtpThread.m_bSendThreadRun = true;

	if( gclsSetupFile.m_strPcapSrtpCrypto.empty() == false && gclsSetupFile.m_strPcapSrtpKey.empty() == false )
	{
		if( gbSrtpInit == false )
		{
			if( srtp_init() != err_status_ok )
			{
				CLog::Print( LOG_ERROR, "%s srtp_init error", __FUNCTION__ );
			}
			else
			{
				gbSrtpInit = true;
			}
		}

		SrtpCreate( gclsSetupFile.m_strPcapSrtpCrypto.c_str(), gclsSetupFile.m_strPcapSrtpKey.c_str(), &psttSrtp );
	}

	pcap_t * psttPcap = pcap_open_offline( gclsSetupFile.m_strPcapFile.c_str(), szErrBuf );
	if( psttPcap == NULL )
	{
		CLog::Print( LOG_ERROR, "%s pcap_open_offline(%s) error(%s)", __FUNCTION__, gclsSetupFile.m_strPcapFile.c_str(), szErrBuf );
	}
	else
	{
		while( gclsRtpThread.m_bStopEvent == false )
		{
			if( GetRtpPacket( psttPcap, szRtpPacket, &iRtpPacketLen ) == false ) break;

			if( psttSrtp )
			{
				iRet = srtp_unprotect( psttSrtp, szRtpPacket, &iRtpPacketLen );
				if( iRet != err_status_ok ) 
				{
					CLog::Print( LOG_ERROR, "srtp_unprotect error" );
					break;
				}
			}

			gettimeofday( &sttTime, NULL );
			iMiliSecond = DiffTimeval( &sttSendTime, &sttTime );

			if( iMiliSecond < iSleepMiliSecond )
			{
				MiliSleep( iSleepMiliSecond - iMiliSecond );
			}

			UdpSend( gclsRtpThread.m_hSocket, szRtpPacket, iRtpPacketLen, gclsRtpThread.m_strDestIp.c_str(), gclsRtpThread.m_iDestPort );

			if( giCodec == 0 || giCodec == 8 )
			{
				iRtpPayloadLen = iRtpPacketLen - sizeof(RtpHeader);
				iSleepMiliSecond = iRtpPayloadLen / 80 * 10;
			}

			sttSendTime = sttTime;
		}

		pcap_close( psttPcap );
	}

	if( psttSrtp )
	{
		srtp_dealloc( psttSrtp );
	}

	CLog::Print( LOG_DEBUG, "%s terminated", __FUNCTION__ );
	gclsRtpThread.m_bSendThreadRun = false;

	StopCall();

	return 0;
}

int GetRtpCodec()
{
	char	szErrBuf[PCAP_ERRBUF_SIZE], szRtpPacket[1600];
	int		iRtpPacketLen;
	int		iCodec = -1;

	pcap_t * psttPcap = pcap_open_offline( gclsSetupFile.m_strPcapFile.c_str(), szErrBuf );
	if( psttPcap == NULL )
	{
		CLog::Print( LOG_ERROR, "%s pcap_open_offline(%s) error(%s)", __FUNCTION__, gclsSetupFile.m_strPcapFile.c_str(), szErrBuf );
	}
	else
	{
		if( GetRtpPacket( psttPcap, szRtpPacket, &iRtpPacketLen ) )
		{
			RtpHeader * psttRtpHeader = (RtpHeader *)szRtpPacket;

			iCodec = psttRtpHeader->GetPT();
		}

		pcap_close( psttPcap );
	}

	return iCodec;
}

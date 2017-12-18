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

#include "SipPlatformDefine.h"
#include "SipCallDumpSetup.h"
#include "ServerService.h"
#include "ServerUtility.h"
#include "Log.h"
#include "pcap.h"
#include "MemoryDebug.h"

class CPacketDumpThreadArg
{
public:
	std::string m_strDevice;
};

THREAD_API PacketDumpThread( LPVOID lpParameter )
{
	CPacketDumpThreadArg * pclsArg = (CPacketDumpThreadArg *)lpParameter;
	pcap_t	* psttPcap = NULL;
	char		szErrBuf[PCAP_ERRBUF_SIZE];
	
	CLog::Print( LOG_INFO, "SipDumpThread( %s ) is started", pclsArg->m_strDevice.c_str() );

	psttPcap = pcap_open_live( pclsArg->m_strDevice.c_str(), gclsSetup.m_iPacketSnapLen, 1, gclsSetup.m_iPacketReadTimeout, szErrBuf );
	if( psttPcap == NULL )
  {
		CLog::Print( LOG_ERROR, "pcap_open_live( %s ) error(%d)", pclsArg->m_strDevice.c_str(), errno );
    goto FUNC_END;
  }

	while( gbStop == false )
	{

	}

FUNC_END:
	if( psttPcap ) pcap_close( psttPcap );

	CLog::Print( LOG_INFO, "SipDumpThread( %s ) is stoped", pclsArg->m_strDevice.c_str() );
	delete pclsArg;

	return 0;
}

bool StartPacketDumpThread( const char * pszDevice )
{
	CPacketDumpThreadArg * pclsArg = new CPacketDumpThreadArg();
	if( pclsArg == NULL )
	{
		return false;
	}
	
	pclsArg->m_strDevice = pszDevice;

	return StartThread( "PacketDumpThread", PacketDumpThread, pclsArg );
}

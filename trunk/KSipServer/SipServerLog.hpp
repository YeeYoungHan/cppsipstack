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

/**
 * @ingroup KSipServer
 * @brief SIP 메시지 전송/수신 이벤트 핸들러
 * @param bSend			전송/수신 구분
 * @param pszPacket SIP 메시지
 * @param pszIp			발신 IP 주소 또는 목적지 IP 주소 
 * @param iPort			발신 포트 번호 또는 목적지 포트 번호
 */
void CSipServer::SipLog( bool bSend, const char * pszPacket, const char * pszIp, int iPort )
{
	CLog::Print( LOG_NETWORK, "%s(%s:%d) [%s]", bSend ? "Send" : "Recv", pszIp, iPort, pszPacket );
}

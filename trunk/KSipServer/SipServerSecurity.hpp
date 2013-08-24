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
 * @brief SIP stack 에서 허용하는 SIP User Agent 인가?
 * @param pszSipUserAgent SIP UserAgent 헤더
 * @returns SIP stack 에서 허용하는 SIP User Agent 이면 true 를 리턴하고 그렇지 않으면 false 한다.
 */
bool CSipServer::IsAllowUserAgent( const char * pszSipUserAgent )
{
	return gclsSetup.IsAllowUserAgent( pszSipUserAgent );
}

/**
 * @brief SIP stack 에서 허용하지 않는 SIP User Agent 인가?
 * @param pszSipUserAgent SIP UserAgent 헤더
 * @returns SIP stack 에서 허용하지 않는 SIP User Agent 이면 true 를 리턴하고 그렇지 않으면 false 한다.
 */
bool CSipServer::IsDenyUserAgent( const char * pszSipUserAgent )
{
	return gclsSetup.IsDenyUserAgent( pszSipUserAgent );
}

/**
 * @brief SIP stack 에서 허용하는 IP 주소인가?
 * @param pszIp 클라이언트 IP 주소
 * @returns SIP stack 에서 허용하는 IP 주소이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServer::IsAllowIp( const char * pszIp )
{
	return true;
}

bool CSipServer::IsDenyIp( const char * pszIp )
{
	return false;
}


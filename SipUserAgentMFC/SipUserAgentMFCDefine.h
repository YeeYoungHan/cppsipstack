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

#ifndef _SIP_USER_AGENT_MFC_DEFINE_H_
#define _SIP_USER_AGENT_MFC_DEFINE_H_

#define SMC_REGISER						10
#define SMC_INCOMING_CALL			21
#define SMC_CALL_RINGING			22
#define SMC_CALL_START				23
#define SMC_CALL_END					24
#define SMC_REINVITE					31
#define SMC_TRANSFER					41
#define SMC_BLIND_TRANSFER		42
#define SMC_TRANSFER_RESPONSE	43
#define SMC_SMS								51		// SIP MESSAGE 요청 수신 이벤트
#define SMC_PRACK							61

#define SMC_RECV_REQUEST			100
#define SMC_RECV_RESPONSE			101

#define WMR_TRUE						0
#define WMR_FALSE						1

#endif

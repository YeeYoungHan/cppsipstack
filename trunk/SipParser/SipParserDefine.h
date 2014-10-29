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

#ifndef _SIP_PARSER_DEFINE_H_
#define _SIP_PARSER_DEFINE_H_

#include "SipPlatformDefine.h"

// ���� SIP �޽��� �Ľ� ����� ����Ϸ��� �ּ��� �����϶�.
#define PARSE_FAST

#define SIP_MESSAGE_MAX_LEN				8192
#define SIP_TAG_MAX_SIZE					21
#define SIP_BRANCH_MAX_SIZE				61
#define SIP_CALL_ID_NAME_MAX_SIZE	51

#define VIA_PREFIX						"z9hG4bK"
#define VIA_PREFIX_LEN				7

#define SIP_PROTOCOL					"sip"
#define SIP_VERSION						"SIP/2.0"

#define SIP_TRANSPORT					"transport"
#define SIP_RECEIVED					"received"
#define SIP_RPORT							"rport"

#define SIP_TRANSPORT_TCP			"tcp"
#define SIP_TRANSPORT_TLS			"tls"

#define SIP_METHOD_INVITE			"INVITE"
#define SIP_METHOD_ACK				"ACK"
#define SIP_METHOD_CANCEL			"CANCEL"
#define SIP_METHOD_BYE				"BYE"
#define SIP_METHOD_REGISTER		"REGISTER"
#define SIP_METHOD_PRACK			"PRACK"
#define SIP_METHOD_REFER			"REFER"
#define SIP_METHOD_INFO				"INFO"
#define SIP_METHOD_NOTIFY			"NOTIFY"
#define SIP_METHOD_MESSAGE		"MESSAGE"
#define SIP_METHOD_OPTIONS		"OPTIONS"
#define SIP_METHOD_SUBSCRIBE	"SUBSCRIBE"

#define SIP_TAG								"tag"
#define SIP_BRANCH						"branch"

#endif

/**
 * @defgroup SipParser SipParser
 * SIP �޽��� �ļ�/���� ���̺귯��
 */


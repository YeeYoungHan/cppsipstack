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

#ifndef _SIP_STACK_DEFINE_H_
#define _SIP_STACK_DEFINE_H_

// TLS ��� ���� SIP stack �� �����ϰ� ������ �Ʒ��� ������ �ּ� ó���ϼ���.
//#define USE_TLS

// MAP ��� HASH MAP �� ����� ��� �Ʒ��� ������ ����ϼ���.
// HASH MAP �� ����ϸ� iterator ���� MAP ���� ���� ������ �ϴ� ������ �����˴ϴ�.
//#define USE_HASH_MAP

#ifdef USE_TLS
// TLS �� ���� ������ ���� �޸� �Ҵ��� �����ϴ� ����� ����ϰ� ���� ���� ��� �ּ� ó���ϼ���.
#define USE_TLS_FREE
#endif

#include "SipParserDefine.h"
#include "SipStackVersion.h"

#define SIP_PACKET_MIN_SIZE		100
#define SIP_PACKET_MAX_SIZE		8192
#define SIP_RING_TIMEOUT			300000

#define SIP_TCP_MAX_SOCKET_PER_THREAD	100
#define SIP_TCP_RECV_TIMEOUT					600
#define SIP_TCP_CONNECT_TIMEOUT				10
#define SIP_TLS_ACCEPT_TIMEOUT				10

#define SIP_USER_AGENT	"CppSipStack_" SIP_STACK_VERSION
#define SIP_MAX_FORWARDS	70

#include "SipMessage.h"

#include <map>

#ifdef USE_HASH_MAP

#ifdef WIN32

#include <hash_map>
#define MAP stdext::hash_map

#else

#include <ext/hash_map>
#define MAP __gnu_cxx::hash_map

namespace __gnu_cxx
{
	template<>
  struct hash< std::string > 
	{
    size_t operator( )( std::string const & s ) const
		{
      return __stl_hash_string( s.c_str( ) );
    }
  };
}

#endif

#else

#define MAP std::map

#endif

void MiliSleep( int iMiliSecond );

// �ڷᱸ�� ����͸�
#define MR_COL_SEP							"|"
#define MR_ROW_SEP							"\n"

#endif

/**
@mainpage C++ SIP Stack
@section intro �Ұ�
	- C++ STL �� ����� SIP Stack ���̺귯��
	- ������Ʈ�� ���� ��ǥ�� IP-PBX ����
	- ������Ʈ Ȩ������ : http://dev.naver.com/projects/sipstack
@section  CREATEINFO �ۼ�����
  - �ۼ��� : �̿���
	- Ȩ������ : http://blog.naver.com/websearch
*/


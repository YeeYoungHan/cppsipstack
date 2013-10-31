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

#ifndef _SIP_USER_AGENT_VERSION_H_
#define _SIP_USER_AGENT_VERSION_H_

#define SIP_USER_AGENT_VERSION "0.17"

/* ���� ����

= ���� 0.17 ( 2013�� 10�� 26�� ) =
 * FreeSwitch �� REGISTER ���� �ʴ� ���׸� ��ġ��
 * StartCall �Է� ���� �м� ����� �߰���
 * SIP REGISTER ���� �޽����� Expires �� �����ϸ� �̸� �����ϴ� ����� �߰���
 * SK ��ε��� IP-PBX �� �����ϱ� ���� ����� �߰���
 * ��ȭ ������ �����ϴ� �޼ҵ带 �߰���
 * ��ü ��ȭ�� �����ϴ� ����� �߰���
 * ��ü ��ȭ�� Call-ID �� �������� ����� �߰���

= ���� 0.16 ( 2013�� 9�� 4�� ) =
 * SIP INVITE �޽����� SDP �� ���������� ���, �̿� ���� ���� �޽����� ���� �õ����� �޸� ������ �߻��ϴ� ���׸� ��ġ��
 * Start �޼ҵ尡 ȣ��� ��쿡�� ���������� Stop �޼ҵ尡 ó���ǵ��� ������
 * callback �������̽��� Start �޼ҵ��� ���ڷ� �߰���
 * callback �����尡 ������ ���� �̺�Ʈ �޼ҵ带 �߰���
 * ���� UserAgent ����� security �������̽��� ������
 * SipUserAgent �� ����� ���� SIP ���� ������ ��� �����ϴ� ����� �߰���
 * �α׾ƿ� ó���Ǵ� �������� �޸� ������ �߻��� �� �ִ� ���׸� ��ġ��
 * CSipCallRtp ��ü�� �������� �ʾƼ� ������ ����Ǵ� ���׸� ��ġ��
 * SIP REGISTER �� ���� 401 ������ ������ ��, ���� ������ ������ SIP REGISTER �޽����� ������ ���� to tag �� �����Ѵ�.
 * credential �� uri �� IP-PBX ������ �ּҸ� �Է��ϵ��� ������.

= ���� 0.13 ( 2013�� 5�� 10�� ) =
 * ACK ���� timeout �� �߻��ϸ� �ش� ��ȭ ������ �����ϴ� ����� �߰���

= ���� 0.12 ( 2013�� 4�� 27�� ) =
 * Delete �޼ҵ� ���ϰ��� ����Ȯ�� ������ ������
 * Via ����� �߰��� ���� ���� �������ݿ� ���� ���� ��Ʈ�� ��Ȯ�ϰ� �Է��ϵ��� ������
 * Stop �޼ҵ� ����� �α׾ƿ�ó���ϴ� ����� �߰���
 * G.723 �ڵ��� ����� �� �ִ� ����� �߰���

= ���� 0.09 ( 2012�� 12�� 04�� ) =
 * TCP �������ݷ� SIP �޽��� ����/���� ��� �߰� �Ϸ�

= ���� 0.08 =
 * SIP MESSAGE ����/���� ����� �߰���.

*/

#endif
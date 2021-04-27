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

#define SIP_USER_AGENT_VERSION "0.35"

/* ���� ����

= ���� 0.35 ( 2021�� 4�� 27�� ) =
 * INVITE �� ���� 302 ���� ������, �ٽ� �����ϴ� INVITE �޽����� To ����� tag �� �߰����� �ʴ´�. (��ī�� IP-PBX ���� ���� ����)

= ���� 0.34 ( 2020�� 10�� 7�� ) =
 * SipUserAgent ������ ���� Dialog �ڷᱸ���� �ʱ�ȭ��Ų��.
 * ��ȭ Hold, Resume �� ���������� �����ϵ��� ������

= ���� 0.33 ( 2010�� 10�� 24�� ) =
 * m_bUseRegisterSession ���� ����� �߰���

= ���� 0.32 ( 2019�� 6�� 19�� ) =
 * CANCEL ������, INVITE 200 OK �� �����ϸ� BYE �� �����Ѵ�.

= ���� 0.31 ( 2019�� 6�� 17�� ) =
 * ������ INVITE �� ���� 200 OK �� ������ ��, ACK �� �������� �� �� ��쿡 �������� ��ȭ ���� �̺�Ʈ�� ȣ����� �ʴ� ������ ������.

= ���� 0.30 ( 2019�� 5�� 28�� ) =
 * qop �� auth-int �� ��� response ��� ������ ������
 * WWW-Authenticate �� qop �� auth,auth-int �� ����� ��쿡 ���������� �����ϵ��� ������.
 * WWW-Authenticate ����� �����ϴ� opaque �� Authorization ����� ������ �ش�.

= ���� 0.29 ( 2019�� 3�� 2�� ) =
 * SendDtmf �޼ҵ带 �߰���
 * IsHold, IsConnected �޼ҵ带 �߰���
 * Delete �޼ҵ带 public ���� ������
 * SIP REGISTER �޽����� ������ ���� Contact ����� ���� TCP/TLS ��Ʈ ��ȣ�� �����ϴ� ����� �߰���

= ���� 0.28 ( 2019�� 1�� 31�� ) =
 * 2XX ���信 ���� ACK Via branch �� ���� �����Ѵ�.
 * 2XX ���信 ���� RecordRoute ��� ������ Dialog �� �����Ѵ�.

= ���� 0.27 ( 2019�� 1�� 25�� ) =
 * INVITE 200 OK ������ Contact ���� ACK req uri �� �����Ѵ�.

= ���� 0.26 ( 2015�� 9�� 16�� ) =
 * SIP REGISTER transaction �� ����Ǳ� ���� ���ο� SIP REGISTER �޽����� ���۵��� �ʴ� ����� �߰���
 * ���� ������ ������ REGISTER �޽����� ���� ������ ��� �α��� ���� ó���Ѵ�.

= ���� 0.25 ( 2015�� 8�� 11�� ) =
 * UDP ��Ʈ�� �������� �ʾƵ� ���������� REGISTER ������ �� �ִ�.

= ���� 0.21 ( 2014�� 6�� 10�� ) =
 * TCP / TLS Ŭ���̾�Ʈ�� �α����ϴ� ����� �߰���
 * PRACK �޽����� ����/�����ϴ� ����� �߰���
 * 100rel ����� �߰���
 * OPTIONS ��û�� ���� 200 ���� ���� ����� �߰���
 * ����͸� ���ڿ��� CMonitorString �� ����ϵ��� ������

= ���� 0.19 ( 2014�� 4�� 19�� ) =
 * CreateCall, StartCall �� ��ȭ ȣ���� �� �ִ� ����� �߰���
 * Stop ȣ�� �ð��� ����
 * RecvRegisterResponse ���� ����� ������ �����Ǿ��� ���� true �� �������� �ʴ� ���׸� ��ġ��
 * SIP register thread �� ������ ���� ������ ����Ǵ� ���׸� ��ġ��
 * CSipCallRtp ���� audio, video port �� �������� �޼ҵ带 �߰���
 * call id �� dialog �� from id �� �������� �޼ҵ带 �߰���
 * �̵�� �� ���� ���� ������ ���뿡�� ������ �� �ֵ��� ������
 * N ���� �̵�� ó���� ���� ����� �߰���

= ���� 0.18 ( 2013�� 10�� 31�� ) =
 * 2�� �̻��� CSipUserAgent �� ������ �� �ִ� ����� �߰���
 * EventThreadEnd �� ���� ���� �Լ����� ���� �Լ��� ������

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

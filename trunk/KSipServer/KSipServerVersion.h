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

#ifndef _K_SIP_SERVER_VERSION_H_
#define _K_SIP_SERVER_VERSION_H_

#define KSIP_SERVER_VERSION	"0.19"

/* ���� ���� 

= ���� 0.21 ( 2014�� 6�� 10�� ) =
 * ����͸� ���ڿ��� CMonitorString �� ����ϵ��� ������

= ���� 0.19 ( 2014�� 4�� 19�� ) =
 * SIP INVITE ��û �޽����� ���� ������ �����ϱ� ���� ��ȭ �ڷᱸ���� �������� �� �Ͽ��� ��ȭ ���� ó���� ���������� ���� �ʴ� ���׸� ��ġ��

= ���� 0.18 ( 2013�� 11�� 18�� ) =
 * Call Pickup ����� �߰���

= ���� 0.17 ( 2013�� 10�� 26�� ) =
 * MySQL �� ������� �ʴ� ��� ���� ������ �߻��ϴ� ���׸� ��ġ��

= ���� 0.16 ( 2013�� 9�� 4�� ) =
 * �α׷���, �α�����ũ�⵵ �ǽð����� ���� �����ϴ�.
 * Ŭ���̾�Ʈ IP ���͸� ����� �߰���
 * ��� User Agent ����� �߰���
 * SIP ���� ���� ��θ� �������� �ʾ��� ��� SipServerMap Load ����� �αװ� ��µ��� �ʵ��� ������
 * �α׾ƿ���Ű�� �� �ϴ� ���׸� ��ġ��

= ���� 0.15 ( 2013�� 8�� 21�� ) =
 * ServerPlatform ���̺귯���� �̿��ϵ��� ������

= ���� 0.14 ( 2013�� 5�� 10�� ) =
 * SIP REGISTER timeout �ð��� �����ϴ� ����� �߰���

= ���� 0.13 ( 2013�� 5�� 10�� ) =
 * SIP transaction list �� �������� ���� SIP User Agent �� �����ϴ� ����� �߰���

= ���� 0.12 ( 2013�� 4�� 27�� ) =
 * ���� IP-PBX ���� XML ���Ͽ� Use Element �� �߰��Ͽ��� ��� ���θ� ������ �� �ִ�
 * KSipServer ���� ���Ͽ��� TLS ���� ������ �߰���

= ���� 0.11 ( 2012�� 12�� 04�� ) =
 * TCP �������ݷ� SIP �޽��� ����/���� ��� �߰� �Ϸ�

= ���� 0.10 ( 2012�� 11�� 21�� ) =
 * SIP REGISTER ���� �޽����� Contact ����� �α��ε� SIP Ŭ���̾�Ʈ ������ �����ϴ� ����� �߰���.

= ���� 0.09 ( 2012�� 10�� 30�� ) =
 * TCP �������ݷ� SIP �޽����� �����ϱ� ���� ���� ������ �߰���.

= ���� 0.08 ( 2012�� 10�� 08�� ) =
 * 1�� �������� �߰�/����/������ IP-PBX ������ �ڷᱸ���� �ݿ��ϴ� ����� �߰���.
 * SIP MESSAGE ���� ����� �߰���.

= ���� 0.07 ( 2012�� 09�� 22�� ) =
 * RTP relay ������� Blind Transfer �� ������ ��, ReINVITE ���۽� SDP �� RTP ��Ʈ�� �� �� �����ϴ� ���׸� ��ġ��.
 * RTP �ڷᱸ���� ���� ����͸� ����� �߰���.
 * MS ������ ���񽺷� ���/������ �� �ִ� ����� �߰���.

= ���� 0.06 ( 2012�� 09�� 13�� ) =
 * CDR ���� ����� �߰���.
 * signal ó�� ����� �߰���.
 * ���μ��� ������ ���� ��� ��ȭ�� �����Ű�� ����� �߰���.
 * RTP relay ��� �߰���.
 * hold / resume ��ɿ��� ReINVITE �޽����� ���� ���۵Ǵ� ���׸� ��ġ��.

= ���� 0.05 ( 2012�� 09�� 06�� ) =
 * �������� ���� ������� INVITE �޽����� ���� 401 ���� �޽����� ������ ��, 180 ���� �޽����� �����ϴ� ���׸� ��ġ��.
 * IP-PBX ������ XML ���Ͽ��� �о �α����ϴ� ����� �߰���.
 * IP-PBX ������ ������ XML ���Ͽ� Call Routing �� ���� ��ȭ��ȣ prefix ����Ʈ�� ������ �� �ִ� ����� �߰���.
 * ����� �ڷᱸ���� �������� �ʴ� ������ ��ȭ��ȣ�� ���� INVITE �޽����� �����Ͽ��� ���� IP-PBX �� Call Route Prefix ����Ʈ�� �˻��ϴ� ����� �߰���.
 * ���������� LocalIp �� �о SIP stack �� ���������� �����ϵ��� ������.

= ���� 0.04 ( 2012�� 08�� 25�� ) =
 * Blind Transfer ����� �߰���.
 * �α��ε� ����ڿ��� �ֱ������� OPTIONS �޽����� �����ϴ� ����� �߰���.
 * �α��ε� ����ڿ��� OPTIONS �޽����� �����ϴ� �ֱ⸦ ���� XML ���� �о� ������ ������.

= ���� 0.03 ( 2012�� 08�� 22�� ) =
 * Screened / Unscreened Transfer ����� �߰���.

= ���� 0.02 ( 2012�� 08�� 18�� ) =
 * MySQL �� �����ϴ� ����� �߰���.
 * ����� ����� ���� ����� �߰���.
 * SIP UserAgent ����� KSipServer �� �߰���.

*/

#endif

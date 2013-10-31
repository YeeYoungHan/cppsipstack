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

#ifndef _SIP_STACK_VERSION_H_
#define _SIP_STACK_VERSION_H_

#define SIP_STACK_VERSION "0.18"

/* ���� ����

= ���� 0.18 ( 2013�� 10�� 31�� ) =
 * CallBackThreadEnd �� ���� ���� �Լ����� ���� �Լ��� ������
 * SipStack.Stop �޼ҵ带 ȣ���ϸ� SipStack callback ����Ʈ�� �ʱ�ȭ��Ų��.

= ���� 0.16 ( 2013�� 9�� 4�� ) =
 * SIP stack �� ������ ��, ��� SIP �޽��� ť�� SIP �޽����� �����ϴ� ����� �߰���
 * callback �����尡 ������ ���� �̺�Ʈ �޼ҵ带 �߰���
 * SIP stack �� ������� ���� Ŭ���̾�Ʈ IP �ּ� ���͸� ����� �߰���
 * ���� UserAgent ����� security �������̽��� ������
 * openssl ���� ������ �� �ִ� ����� �߰���

= ���� 0.13 ( 2013�� 5�� 10�� ) =
 * SIP transaction list �� �������� ���� SIP User Agent ����Ʈ ���� ����� �߰���
 * SIP �޽����� ������ ���� compact form ���� �������� �����ϴ� ����� �߰���

= ���� 0.12 ( 2013�� 4�� 27�� ) =
 * ACK ���� timeout callback ����� �߰���
 * SipStack �� stack ���� ���ŵ� ���� ���������� ���� ���ŵǾ �߻��ϴ� ������ ���� ���׸� ��ġ��
 * ���� �������ݿ� ���� ���� ��Ʈ ��ȣ�� �������� �޼ҵ带 �߰���
 * Via ����� �߰��� ���� ���� �������ݿ� ���� ���� ��Ʈ�� ��Ȯ�ϰ� �Է��ϵ��� ������.
 * IST, NICT, NIST �� ���Ե� call-id ���ڿ��� �������� ����� �߰���
 * SIP Reason ��� �ļ�/���� Ŭ������ �߰���
 * SIP �޽����� ������ Ŭ���̾�Ʈ IP / Port �� SIP �޽����� �����ϴ� ����� �߰���
 * AddSipParameter �޼ҵ带 InsertSipParameter �޼ҵ�� �̸��� ������
 * UpdateSipParameter �޼ҵ带 �߰���
 * TLS ������� SIP ��û�� ���� ���� �޽����� ���۵��� �ʴ� ���׸� ��ġ��
 * TLS ������ ����� �� �ֵ��� ������

= ���� 0.07 ( 2012�� 12�� 04�� ) =
 * TCP �������ݷ� SIP �޽��� ����/���� ��� �߰� �Ϸ�

= ���� 0.06 ( 2012�� 10�� 30�� ) =
 * TCP �������ݷ� SIP �޽��� ����/���� ��� �߰���

= ���� 0.05 ( 2012�� 10�� 21�� ) =
 * VIA branch �� time ������ �־ SIP �޽����� ��ġ�� Ȯ���� ����.
 * VIA branch �� Call-Id �� system unique string �� �ִ� ����� �߰���.

= ���� 0.04 ( 2012�� 08�� 21�� ) =
 * SendSipMessage �޼ҵ忡�� SIP stack �� CSipMessage �� �Է����� �� �ϸ� CSipMessage �� �����ϵ��� ������.
 * SendSipMessage �޼ҵ� ȣ�� ���нÿ� CSipMessage �� �����ϴ� �ڵ带 ������.

= ���� 0.03 ( 2012�� 08�� 18�� ) =
 * UserAgent ����� ���� ���α׷����� ������ �� �ִ� ����� �߰���
 * SIP �޽����� �����ϱ� ���� 2�� SIP �޽����� �����ϴ� ����� 1���� �����ϵ��� ������.

*/

#endif

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

package com.cppsipstack;

/** SipUserAgent �� �̺�Ʈ ���ſ� callback interface
 * @author websearch (websearch@naver.com)
 */
public interface SipUserAgentCallBack
{
	/** SIP REGISTER ���� �޽��� ���� �̺�Ʈ �ڵ鷯
	 * @param clsInfo	SIP REGISTER ���� �޽����� ������ IP-PBX ���� ���� ��ü
	 * @param iStatus SIP REGISTER ���� �ڵ�
	 */
	public void EventRegister( SipServerInfo clsInfo, int iStatus );
	
	/** SIP ��ȭ ��û ���� �̺�Ʈ �ڵ鷯
	 * @param strCallId	SIP Call-ID
	 * @param strFrom		SIP From ����� ���̵�
	 * @param strTo			SIP To ����� ���̵�
	 * @param clsRtp		RTP ���� ���� ��ü
	 */
	public void EventIncomingCall( String strCallId, String strFrom, String strTo, SipCallRtp clsRtp );
	
	/** SIP Ring / Session Progress ���� �̺�Ʈ �ڵ鷯
	 * @param strCallId		SIP Call-ID
	 * @param iSipStatus	SIP ���� �ڵ�
	 * @param clsRtp			RTP ���� ���� ��ü
	 */
	public void EventCallRing( String strCallId, int iSipStatus, SipCallRtp clsRtp );
	
	/** SIP ��ȭ ���� �̺�Ʈ �ڵ鷯
	 * @param strCallId	SIP Call-ID
	 * @param clsRtp		RTP ���� ���� ��ü
	 */
	public void EventCallStart( String strCallId, SipCallRtp clsRtp );
	
	/** SIP ��ȭ ���� �̺�Ʈ �ڵ鷯
	 * @param strCallId		SIP Call-ID
	 * @param iSipStatus	SIP ���� �ڵ�. INVITE �� ���� ���� �������� ��ȭ�� ����� ���, INVITE �� ���� �ڵ带 �����Ѵ�.
	 */
	public void EventCallEnd( String strCallId, int iSipStatus );
	
	/** SIP ReINVITE ���� �̺�Ʈ �ڵ鷯
	 * @param strCallId	SIP Call-ID
	 * @param clsRtp		RTP ���� ���� ��ü
	 */
	public void EventReInvite( String strCallId, SipCallRtp clsRtp );
	
	/** Blind Transfer ��û ���� �̺�Ʈ �ڵ鷯
	 * @param strCallId			SIP Call-ID
	 * @param strReferToId	��ȭ ��ȯ�� ���� ���̵�
	 */
	public void EventBlindTransfer( String strCallId, String strReferToId );
}

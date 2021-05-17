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

import java.io.Serializable;

/** RTP ���� ���� Ŭ����
 * @ingroup SipUserAgentAndroid
 * @author websearch (websearch@naver.com)
 */
public class SipCallRtp implements Serializable
{
	private static final long serialVersionUID = 1L;

	public final int E_RTP_SEND_RECV = 0;
	public final int E_RTP_SEND = 1;
	public final int E_RTP_RECV = 2;
	public final int E_RTP_INACTIVE = 3;
	
	/** IP �ּ� */
	public String m_strIp;
	
	/** ��Ʈ ��ȣ */
	public int m_iPort;
	
	/** ���õ� �ڵ� ��ȣ */
	public int m_iCodec = 0;
	
	/** ����/���� */
	public int m_iDirection = 0;
}

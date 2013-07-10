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

/** SipUserAgent 의 이벤트 수신용 callback interface
 * @author websearch (websearch@naver.com)
 */
public interface SipUserAgentCallBack
{
	public void EventRegister( SipServerInfo clsInfo, int iStatus );
	public void EventIncomingCall( String strCallId, String strFrom, String strTo, SipCallRtp clsRtp );
	public void EventCallRing( String strCallId, int iSipStatus, SipCallRtp clsRtp );
	public void EventCallStart( String strCallId, SipCallRtp clsRtp );
	public void EventCallEnd( String strCallId, int iSipStatus );
}

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

import android.util.Log;

/** 안드로이드 로그 출력 클래스
 * @author websearch (websearch@naver.com)
 *
 */
public class SipLog
{
	public static String LOG_TAG = "AndroidSipStack";
	
	/** 디버그 로그를 출력한다.
	 * @param strText	로그 메시지
	 */
	public static void Debug( String strText )
	{
		Log.d( LOG_TAG, strText );
	}
	
	/** 에러 로그를 출력한다.
	 * @param strText 로그 메시지
	 */
	public static void Error( String strText )
	{
		Log.e( LOG_TAG, strText );
	}
}

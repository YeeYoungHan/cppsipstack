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

package com.example.testandroidsipstack;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;

/** 경고창 보여주기
 * @author 이영한 ( http://blog.naver.com/websearch )
 */
public class Alert
{
	public static void ShowError( Context clsContext, String strMessage )
	{
		new AlertDialog.Builder( clsContext ).setTitle( "Error" ).setMessage( strMessage ).setPositiveButton( "OK"
				, new DialogInterface.OnClickListener( )
					{
						public void onClick( DialogInterface dialog, int which )
						{
							dialog.dismiss( );
						}
					} ).show( );
	}
}

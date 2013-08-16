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

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.EditText;

/** SIP 서버 및 사용자 아이디/비밀번호 설정 Activity
 * @author 이영한 ( http://blog.naver.com/websearch )
 */
public class SetupActivity extends Activity implements OnClickListener
{
	EditText m_txtSipServerIp, m_txtSipServerPort, m_txtSipDomain, m_txtSipUserId, m_txtSipPassWord;
	
	@Override
	protected void onCreate( Bundle savedInstanceState )
	{
		super.onCreate( savedInstanceState );
		setContentView( R.layout.activity_setup );
		
		m_txtSipServerIp = (EditText)findViewById( R.id.txtSipServerIp );
		m_txtSipServerPort = (EditText)findViewById( R.id.txtSipServerPort );
		m_txtSipDomain = (EditText)findViewById( R.id.txtSipDomain );
		m_txtSipUserId = (EditText)findViewById( R.id.txtSipUserId );
		m_txtSipPassWord = (EditText)findViewById( R.id.txtSipPassWord );
		
		findViewById( R.id.btnOk ).setOnClickListener( this );
		findViewById( R.id.btnCancel ).setOnClickListener( this );
		
		m_txtSipServerIp.setText( Setup.m_strSipServerIp );
		m_txtSipServerPort.setText( Setup.m_iSipServerPort );
		m_txtSipDomain.setText( Setup.m_strSipDomain );
		m_txtSipUserId.setText( Setup.m_strSipUserId );
		m_txtSipPassWord.setText( Setup.m_strSipPassWord );
	}

	@Override
	public void onClick( View v )
	{
		switch( v.getId( ) )
		{
		case R.id.btnOk:
			{
				String strSipServerIp = m_txtSipServerIp.getText( ).toString( );
				if( strSipServerIp.length( ) == 0 )
				{
					Alert.ShowError( this, "SIP Server IP is empty" );
					return;
				}
				
				String strSipServerPort = m_txtSipServerPort.getText( ).toString( );
				int iSipServerPort = 0;
				
				try
				{
					iSipServerPort = Integer.parseInt( strSipServerPort );
				}
				catch( Exception e )
				{
				}
				
				if( iSipServerPort <= 0 || iSipServerPort > 65535 )
				{
					Alert.ShowError( this, "SIP Server Port is invalid" );
					return;
				}
				
				String strSipDomain = m_txtSipDomain.getText( ).toString( );
				
				String strSipUserId = m_txtSipUserId.getText( ).toString( );
				if( strSipUserId.length( ) == 0 )
				{
					Alert.ShowError( this, "SIP User ID is empty" );
					return;
				}
				
				String strSipPassWord = m_txtSipPassWord.getText( ).toString( );
				
				Setup.m_strSipServerIp = strSipServerIp;
				Setup.m_iSipServerPort = iSipServerPort;
				Setup.m_strSipDomain = strSipDomain;
				Setup.m_strSipUserId = strSipUserId;
				Setup.m_strSipPassWord = strSipPassWord;
				
				Setup.Put( this );
				
				finish();
			}
			break;
		case R.id.btnCancel:
			finish();
			break;
		}
	}
}

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

#include "SipReason.h"
#include <string.h>

bool TestSipReason()
{
	CSipReason clsReason;
	std::string strText = "Control;cause=200;text=\"ok\";charset=euc-kr";
	std::string strValue;

	if( clsReason.Parse( strText.c_str(), (int)strText.length() ) == -1 ) return false;

	if( strcmp( clsReason.m_strProtocol.c_str(), "Control" ) ) return false;

	if( clsReason.SelectParam( "cause", strValue ) == false ) return false;
	if( strcmp( strValue.c_str(), "200" ) ) return false;

	if( clsReason.SelectParam( "text", strValue ) == false ) return false;
	if( strcmp( strValue.c_str(), "ok" ) ) return false;

	if( clsReason.SelectParam( "charset", strValue ) == false ) return false;
	if( strcmp( strValue.c_str(), "euc-kr" ) ) return false;

	return true;
}

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

#include "StringUtility.h"

bool TestString()
{
	std::string	strText = "dscp;app=36;msg=36;hotline=46;presence=36; broadcast=46;ppt=";
	std::string strValue;
	int	iValue;

	if( SearchValue( strText, "app=", ';', strValue ) == false ) return false;
	if( strcmp( strValue.c_str(), "36" ) ) return false;

	if( SearchValue( strText, "broadcast=", ';', strValue ) == false ) return false;
	if( strcmp( strValue.c_str(), "46" ) ) return false;

	if( SearchValue( strText, "test=", ';', strValue ) == true ) return false;
	if( SearchValue( strText, "test=", ';', iValue ) == true ) return false;

	if( SearchValue( strText, "app=", ';', iValue ) == false ) return false;
	if( iValue != 36 ) return false;

	if( SearchValue( strText, "broadcast=", ';', iValue ) == false ) return false;
	if( iValue != 46 ) return false;

	if( SearchValue( strText, "ppt=", ';', iValue ) == false ) return false;
	if( iValue != 0 ) return false;

	return true;
}

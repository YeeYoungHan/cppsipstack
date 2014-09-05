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

static bool TestLeftTrim( const char * pszInput, const char * pszOutput )
{
	std::string strText = pszInput;

	LeftTrimString( strText );
	
	if( strcmp( pszOutput, strText.c_str() ) )
	{
		printf( "%s input(%s) output(%s) => result(%s)", __FUNCTION__, pszInput, pszOutput, strText.c_str() );
		return false;
	}

	return true;
}

bool TestStringUtility()
{
	if( TestLeftTrim( "abcd", "abcd" ) == false ) return false;
	if( TestLeftTrim( " abcd", "abcd" ) == false ) return false;
	if( TestLeftTrim( "  abcd", "abcd" ) == false ) return false;
	if( TestLeftTrim( " \t abcd", "abcd" ) == false ) return false;

	return true;
}

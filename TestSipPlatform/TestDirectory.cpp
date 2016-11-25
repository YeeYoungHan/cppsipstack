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

#include "Directory.h"

bool TestDirectory()
{
	bool bRes = false;

#ifdef WIN32
	bRes = CDirectory::IsDirectory( "c:\\temp" );
#else
	bRes = CDirectory::IsDirectory( "/tmp" );
#endif
	if( bRes == false ) return false;

#ifdef WIN32
	bRes = CDirectory::IsDirectory( "c:\\Windows\\write.exe" );
#else
	bRes = CDirectory::IsDirectory( "/etc/hosts" );
#endif
	if( bRes ) return false;

	CDirectory::Create( "c:\\temp\\log\\test\\me2" );

	uint64_t iTotalSize;	

#ifdef WIN32
	iTotalSize = CDirectory::GetSize( "c:\\temp" );
#else
	iTotalSize = CDirectory::GetSize( "/tmp" );
#endif

	printf( "directory size = " UNSIGNED_LONG_LONG_FORMAT "\n", iTotalSize );

	std::string strFileName;

#ifdef WIN32
	CDirectory::GetFileName( "c:\\temp\\test\\me.pcap", strFileName );
#else
	CDirectory::GetFileName( "/temp/test/me.pcap", strFileName );
#endif
	if( strcmp( strFileName.c_str(), "me.pcap" ) )
	{
		printf( "CDirectory::GetFileName error\n" );
		return false;
	}

	return true;
}

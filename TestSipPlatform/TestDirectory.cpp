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
#include "TestSipPlatform.h"

bool TestDirectoryWin32()
{
	Check( CDirectory::IsDirectory( "c:\\Windows" ) );
	Check( CDirectory::IsDirectory( "c:\\Windows\\write.exe" ) == false );

	if( CDirectory::IsDirectory( "c:\\temp" ) )
	{
		if( CDirectory::IsDirectory( "c:\\temp\\testdirectory" ) == false )
		{
			Check( CDirectory::Create( "c:\\temp\\testdirectory\\test\\me2" ) );
			Check( CDirectory::Create( "c:\\temp\\testdirectory\\test\\me2" ) );
			Check( CDirectory::IsDirectory( "c:\\temp\\testdirectory\\test\\me2" ) );
			Check( CDirectory::Delete( "c:\\temp\\testdirectory" ) );
			Check( CDirectory::IsDirectory( "c:\\temp\\testdirectory" ) == false );
		}
	}

	uint64_t iTotalSize;

	iTotalSize = CDirectory::GetSize( "c:\\temp" );

	std::string strFileName;

	CDirectory::GetFileName( "c:\\temp\\test\\me.pcap", strFileName );
	Check( !strcmp( strFileName.c_str(), "me.pcap" ) );

	return true;
}

bool TestDirectoryLinux()
{
	Check( CDirectory::IsDirectory( "/tmp" ) );
	Check( CDirectory::IsDirectory( "/etc/hosts" ) == false );

	uint64_t iTotalSize;	

	iTotalSize = CDirectory::GetSize( "/tmp" );

	std::string strFileName;

	CDirectory::GetFileName( "/temp/test/me.pcap", strFileName );
	Check( !strcmp( strFileName.c_str(), "me.pcap" ) );

	return true;
}

bool TestDirectory()
{
#ifdef WIN32
	return TestDirectoryWin32();
#else
	return TestDirectoryLinux();
#endif
}

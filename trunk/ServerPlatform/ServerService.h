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

#ifndef _SERVER_SERVICE_H_
#define _SERVER_SERVICE_H_

#include <string>

class CServerService
{
public:
	std::string m_strName;
	std::string m_strDisplayName;
	std::string m_strDescription;
	std::string m_strVersion;
	std::string m_strConfigFileName;
};

typedef int (*ServerFunc)();

int ServerMain( int argc, char * argv[], CServerService & clsService, ServerFunc pFunc );
char * GetConfigFileName();
void ServerSignal();

#endif

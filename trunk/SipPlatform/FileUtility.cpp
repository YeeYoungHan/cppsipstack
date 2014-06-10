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

#include "FileUtility.h"
#include <sys/stat.h>

#include "MemoryDebug.h"

/**
 * @ingroup SipPlatform
 * @brief ������ �����ϴ��� �˻��Ѵ�.
 * @param pszFileName ���� �̸�
 * @returns ������ �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool IsExistFile( const char * pszFileName )
{
	struct stat sttStat;

	if( stat( pszFileName, &sttStat ) == -1 )
	{
		return false;
	}

	return true;
}

/**
 * @ingroup SipPlatform
 * @brief ���� ũ�⸦ �����Ѵ�.
 * @param pszFileName ���� �̸�
 * @returns ������ �����ϸ� ���� ũ�⸦ �����ϰ� �׷��� ������ 0 �� �����Ѵ�.
 */
int64_t GetFileSize( const char * pszFileName )
{
	struct stat sttStat;

	if( stat( pszFileName, &sttStat ) == -1 )
	{
		return 0;
	}

	return sttStat.st_size;
}

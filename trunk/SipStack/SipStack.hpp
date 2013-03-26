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

/**
 * @ingroup SipStack
 * @brief �����带 �����Ѵ�.
 * @param pszName					������ �̸�
 * @param lpStartAddress	������ �Լ�
 * @param lpParameter			������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
#ifdef WIN32
bool StartThread( const char * pszName, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter )
{
	DWORD		dwThreadId;
	HANDLE	hThread;

	hThread = CreateThread( NULL, 0, lpStartAddress, lpParameter, 0, &dwThreadId );
	if( hThread == NULL )
	{
		return false;
	}

	CloseHandle( hThread );
	
	return true;
}
#else
bool StartThread( const char * pszName, void *(*lpStartAddress)(void*), void * lpParameter )
{
	pthread_t	iThread;

	if( pthread_create( &iThread, NULL, lpStartAddress, lpParameter ) != 0 )
	{
		return false;
	}

	pthread_detach( iThread );

	return true;
}
#endif
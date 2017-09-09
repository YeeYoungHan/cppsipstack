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

#ifndef _MEMORY_DEBUG_H_
#define _MEMORY_DEBUG_H_

// 메모리 누수 검사를 위해서 아래의 주석을 해제한 후, 빌드하고 실행 & 종료하면 메모리 누수가 발생한 소스 코드가 출력된다.
//#define MEMORY_DEBUG

#ifdef WIN32
#ifdef _DEBUG
#ifdef MEMORY_DEBUG

#ifdef malloc
#undef malloc
#endif

#define malloc(s) (_malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__ ))

#ifdef calloc
#undef calloc
#endif

#define calloc(c, s) (_calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__ ))

#ifdef realloc
#undef realloc
#endif

#define realloc(p, s) (_realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__ ))

#ifdef _expand
#undef _expand
#endif

#define _expand(p, s) (_expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__  ))

#ifdef free
#undef free
#endif

#define free(p) (_free_dbg(p, _NORMAL_BLOCK))

#ifdef _msize
#undef _msize
#endif

#define _msize(p) (_msize_dbg(p, _NORMAL_BLOCK))

#define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )

#endif
#endif
#endif

#endif

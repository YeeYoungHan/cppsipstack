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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <jni.h>
#include "AndroidLog.h"

JavaVM * gjVm;

/** JNI_OnLoad */
#ifdef WIN32
JNIEXPORT jint JNICALL JNI_OnLoad( JavaVM *vm, void *reserved )
#else
jint JNI_OnLoad( JavaVM * vm, void* reserved )
#endif
{
  if( !vm )
  {
		AndroidErrorLog( "JNI_OnLoad did not receive a valid VM pointer" );
    return -1;
  }

	gjVm = vm;
  
  // Get JNI
  JNIEnv * env;
  if( JNI_OK != vm->GetEnv( reinterpret_cast<void**>(&env), JNI_VERSION_1_4 ) )
  {
    AndroidErrorLog( "JNI_OnLoad could not get JNI env" );
    return -1;
  }
  
  return JNI_VERSION_1_4;
}


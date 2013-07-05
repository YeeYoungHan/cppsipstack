#include "AndroidPut.h"
#include "AndroidLog.h"
#include <string.h>

bool PutString( JNIEnv * env, jobject jObject, jclass jClass, const char * pszName, const char * pszValue )
{
	jstring strValue = env->NewStringUTF( pszValue );
	if( strValue == NULL )
	{
		AndroidErrorLog( "PutString(%s) NewStringUTF error", pszName );
		return false;
	}

	jfieldID fid = env->GetFieldID( jClass, pszName, "Ljava/lang/String;" );
	env->SetObjectField( jObject, fid, strValue );

	env->DeleteLocalRef( strValue );

	return true;
}
